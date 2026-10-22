# FPV Kamikaze Drone — OSD Mod Architecture & Comprehensive Technical Dump

> **For AI Agents & Developers**: This document contains the complete technical architecture, reverse-engineering findings, resolved bugs, unresolved challenges, and exact memory layouts for the FPV Kamikaze Drone custom military OSD mod.

---

## 1. Project Overview & Objective

### 1.1 Goal
Replace the default in-game HUD of the **FPV Kamikaze Drone** game with an authentic Russian military FPV drone OSD layout (sourced from real combat footage, reference asset: `media_1791484815764_4a562d7e.png`, 1024x582 RGBA PNG with alpha transparency).

### 1.2 Target Platform & Architecture
- **Game**: *FPV Kamikaze Drone*
- **Game Engine**: Unreal Engine 5.5.x (Shipping 64-bit build)
- **Renderer**: DirectX 12 (D3D12)
- **Archive Format**: Unreal Engine IoStore (`.utoc` + `.ucas`) + Pak file (`FPVKamikazeDrone-Windows.pak`)
- **Main Game Executable**: `FPVKamikazeDrone\Binaries\Win64\FPVKamikazeDrone-Win64-Shipping.exe`
- **Bootstrapper**: `FPVKamikazeDrone.exe` (169 KB launcher)
- **OS**: Windows 10/11 x64

---

## 2. Directory Structure & Key Files

```
d:\Zona Downloads\FPVKamikazeDrone-1\
│
├── CLAUDE.md                               # This documentation file
├── Start_Game_With_OSD.bat                 # One-click launcher (starts overlay, then game)
│
├── OSD_Overlay\                            # The active mod project (C++ / Direct2D)
│   ├── main.cpp                            # Complete overlay source code
│   ├── FPV_OSD_Overlay.exe                 # Compiled 64-bit release binary
│   ├── osd_template.png                    # Clean 1024x582 RGBA template (transparent)
│   ├── osd_template_1080p.png              # Scaled 1080p template
│   ├── diag_game.cpp                       # Memory scanner diagnostic tool
│   ├── diag_game.exe                       # Compiled diagnostic executable
│   └── osd_log.txt                         # Runtime debug log output
│
├── FPVKamikazeDrone\
│   ├── Binaries\Win64\
│   │   ├── FPVKamikazeDrone-Win64-Shipping.exe   # Actual 64-bit game process
│   │   └── EOSSDK-Win64-Shipping.dll             # Epic Online Services DLL
│   └── Content\Paks\
│       ├── FPVKamikazeDrone-Windows.utoc          # IoStore Table of Contents (6.35 MB)
│       ├── FPVKamikazeDrone-Windows.ucas          # IoStore Chunks Container (8.43 GB)
│       └── FPVKamikazeDrone-Windows.pak           # Pak file (89.5 MB)
│
├── fmodel\                                 # FModel IoStore extraction setup
│   ├── FModel.exe                          # FModel GUI tool configured for GAME_UE5_5
│   └── Output\.data\
│       └── oodle-data-shared.dll           # Oodle decompression library
│
└── UE4SS\                                  # Incompatible / abandoned inject loader
    └── UE4SS.dll                           # DO NOT USE: Crashes UE 5.5 on startup
```

---

## 3. Telemetry & OSD Specification

### 3.1 Visual Layout (Matching `osd_template.png` 1024x582)
The OSD template is rendered at **1024x582** native resolution and upscaled using nearest-neighbor filtering to match retro video feed pixelation:

| Field | Position (1024x582) | Description | Dynamic Source |
|---|---|---|---|
| **ACRO** | Top-Left (static) | Flight mode banner | Built into template PNG |
| **Flight Time** | `(16, 44)` | Format `MM:SS` (e.g. `24:29`) | Increments every 1s while in flight |
| **Altitude** | `(18, 82)` | Format `+0.0` or `-21.4` (meters) | Camera `POV.Location.Z / 100.0` or climb rate |
| **Total Voltage** | `(430, 22)` | Format `22.2V` (6S LiPo total) | Procedural battery model with sag |
| **Cell Voltage** | `(565, 22)` | Format `3.70V` (Volts per cell) | `vTotal / 6.0f` |
| **PITCH** | `(680, 40)` | Format `29.5` (degrees) | Drone nose tilt: `-85.0` to `+85.0` |
| **Amperes** | `(836, 324)` | Format `22.4` (Amperes) | Non-linear load: `baseA + (throttle^2)*maxA` |
| **Capacity** | `(836, 357)` | Format `30250` (mAh) | Cumulative current integration over time |
| **RXg** | `(78, 410)` | `-6` or `-7` (dBm) | Fluctuate randomly between `-6` and `-7` |
| **ГАЗ (Throttle)**| `(892, 410)`| `0` to `100` (%) | Current throttle axis input |
| **TXg** | `(70, 445)` | `0.5` to `1.0` (Watts) | Procedural smooth float drift |

---

## 4. Reverse-Engineering Findings (UE 5.5)

Through binary disassembly of `FPVKamikazeDrone-Win64-Shipping.exe`, the following global addresses and structure layouts were identified:

### 4.1 Global RVAs (Base Image: `FPVKamikazeDrone-Win64-Shipping.exe`)
- **`GWorld`**: `ImageBase + 0x0a219d88`
- **`GUObjectArray` (Object Table)**: `ImageBase + 0x09db24e0`
- **`GUObjectArray` (Element Count)**: `ImageBase + 0x09db2504`
- **`FNamePool` (GNames Table)**: `ImageBase + 0x09fab2c0`

### 4.2 `GUObjectArray` Memory Layout
Verified from machine instructions at RVA `0x1446424`:
```assembly
mov eax, [rdi + 0x0C]   ; InternalIndex
cmp eax, [0x9db2504]    ; NumElements
jge out_of_bounds
mov ecx, eax
movzx eax, ax           ; ElementOffset = Index & 0xFFFF
shr rcx, 16             ; ChunkIndex = Index >> 16
lea rdx, [rax + rax*2]  ; rdx = eax * 3
mov rax, [0x9db24e0]    ; rax = Objects (ChunkTable**)
mov rcx, [rax + rcx*8]  ; rcx = Objects[ChunkIndex]
lea rcx, [rcx + rdx*8]  ; rcx = &Chunk[ElementOffset * 24]
```
- **Chunk Size**: `65,536` (`0x10000`) elements per chunk.
- **`sizeof(FUObjectItem)`**: `24` bytes (`0x18`).
  - `+0x00`: `UObject* Object`
  - `+0x08`: `int32 Flags`
  - `+0x0C`: `int32 ClusterRootIndex`
  - `+0x10`: `int32 SerialNumber`

### 4.3 `UObject` & Class Resolution
- `+0x00`: `void* VTable`
- `+0x08`: `int32 ObjectFlags`
- `+0x0C`: `int32 InternalIndex`
- `+0x10`: `UClass* ClassPrivate`
- `+0x18`: `FName NamePrivate` (`uint32 ComparisonIndex`, `uint32 Number`)

### 4.4 `FNamePool` (UE 5.5)
- Chunk block pointers start at `FNamePool + 0x10`.
- Block lookup: `BlockIndex = compIndex >> 16`, `BlockOffset = (compIndex & 0xFFFF) * 2`.
- Entry header: `uint16 Header = *(uint16*)(Block + BlockOffset)`.
- Length: `Header >> 6`.
- Is Wide (UTF-16): `Header & 1`.
- String data starts at `Block + BlockOffset + 2`.

### 4.5 Asset Names Discovered via IoStore Directory Index
- **Kamikaze Drone**:
  - Pawn Class: `BP_Drone_FPV_C`, `BP_Kamikaze_C`
  - In-Game HUD: `WBP_FPV_Drone_C`, `BP_UI_FPV`
  - Built-in Reticle/Crosshair Texture: `A_Aim`
  - Built-in Battery Icon: `A_FPV_Battery`, `A_FPV_Battery_Full`
- **Recon / Spectator Drone (Разведчик / Мавик)**:
  - Pawn Class: `BP_Spectator_C`, `BP_Striker_C`
  - Recon HUD: `WBP_Striker_C`
  - Mesh / Props: `SK_Drone_Mavic`, `A_MavicPropv2`, `A_Mavic_Motor`

---

## 5. Architectural Approaches & Evolution

### Attempt 1: In-Process Hooking via UE4SS (Failed & Removed)
- **Idea**: Inject `UE4SS.dll` via `dwmapi.dll` into the game directory to execute Lua scripts and hide UMG widgets internally.
- **Result**: Instant crash on game boot (`STATUS_ACCESS_VIOLATION 0xC0000005`).
- **Cause**: Existing UE4SS v3 release is incompatible with Unreal Engine 5.5 IoStore ZenLoader and D3D12 pipeline.
- **Decision**: Avoid all in-process DLL injection to ensure 100% crash immunity. Shifted to an external transparent overlay process.

### Attempt 2: Direct2D Hardware-Accelerated Overlay (Current Foundation)
- **Implementation**: `OSD_Overlay\main.cpp` compiles to `FPV_OSD_Overlay.exe`.
- **Technique**: Win32 Layered Window (`WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST`) using `UpdateLayeredWindow` with `AC_SRC_ALPHA`.
- **Synchronization**: Locates `FPVKamikazeDrone-Win64-Shipping.exe` window, tracks client coordinates via `ClientToScreen`, and draws at ~60 FPS.
- **Lifecycle**: Automatically terminates when the game process terminates.

---

## 6. Solved Issues & Fixes Implemented

### Fix 1: Resolution & Pixel Density Discrepancy (Solved)
- **Problem**: Dynamic numbers rendered at crisp 1080p, looking mismatched against the low-res pixelated OSD template.
- **Fix**: Rendered both template and text onto an offscreen `ID2D1BitmapRenderTarget` (`1024x582`). Applied Consolas Bold with 4-pass 1px outline, then blitted to the full screen DC with `D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR`. All elements now share identical pixel density.

### Fix 2: Ugly Black Masking Rectangles (Solved & Reverted)
- **Problem**: In an attempt to cover the old HUD, black fill rectangles (`FillRectangle` with `g_pMaskBrush`) were drawn on the overlay, creating visible black censor boxes (`media_1791491058774_bb03147d.jpg`).
- **Fix**: Completely deleted `g_pMaskBrush` and all `FillRectangle` calls. The overlay is now 100% clean alpha transparency.

### Fix 3: Pitch Coupled to Throttle / Key Mappings (Solved)
- **Problem**: `GetAsyncKeyState('W')` was mapped to throttle, but in FPV drone flight, `W` is nose-down pitch (forward tilt). This caused pressing `W` to drive throttle while freezing or distorting pitch.
- **Fix**:
  - **Pitch (тангаж)** is strictly nose inclination:
    - Keyboard: `W` or `Up Arrow` tilts nose down (`pitch -= delta`), `S` or `Down Arrow` tilts nose up (`pitch += delta`).
    - Mouse: pushing forward tilts nose down, pulling back tilts nose up.
    - Controller / Radio: Right stick Y tilts nose up/down.
    - Pitch inputs have **zero effect on throttle**.
  - **Throttle (газ)** is strictly thrust:
    - Keyboard: `Shift` / `Space` increases throttle.
    - Controller / Radio: Left stick Y / Right trigger.
    - Throttle inputs have **zero effect on pitch**.

### Fix 4: Main Menu & Pause Suppression (Solved)
- **Problem**: The overlay was displaying over main menu buttons and pause menus.
- **Fix**: Implemented `GetCursorInfo(&ci)`. In UE5, during flight the OS cursor is hidden (`(ci.flags & CURSOR_SHOWING) == 0`). In menus and pause screens, the cursor is made visible. When cursor is visible, the overlay immediately hides (`SW_HIDE`).

---

## 7. Open / Unresolved Challenges & Critical Bugs

### Bug 1: Automatic Drone Differentiation (Kamikaze vs Recon Drone)
- **Symptom**: The overlay appears on both the Kamikaze drone and the Recon drone (разведчик / Mavic / Striker). The user demands that the OSD appear **ONLY** on the Kamikaze drone.
- **Root Cause Analysis**:
  - Both drones hide the mouse cursor during flight, so `!cursorShowing` is true for both.
  - An attempt was made to traverse `GWorld -> OwningGameInstance -> LocalPlayers[0] -> PlayerController -> AcknowledgedPawn -> ClassPrivate` to read the class name.
  - **Fatal Flaw in previous build**: `g_telemetry.isKamikaze` was defaulted to `false`. When the pointer chain failed to resolve at runtime, `isKamikaze` stayed `false`, causing the overlay to disappear completely ("теперь вообще нет нового осд").
- **Current State**:
  - `g_telemetry.isKamikaze` defaults to `true` (fail-safe so overlay does not vanish).
  - Memory scanner searches `GUObjectArray` for `BP_Spectator_C`, `BP_Striker_C`, `WBP_Striker_C`.
  - **What still needs improvement**: Reliable identification of the active possessed pawn in memory without false positives or negatives, ensuring instant hiding when piloting the Recon drone.

### Bug 2: Complete Suppression of Built-In Kamikaze HUD
- **Symptom**: The game's original white telemetry text, crosshair (`A_Aim`), and battery icon (`A_FPV_Battery`) are rendered underneath the modded OSD.
- **Attempted Solutions**:
  1. *Black masking boxes*: Created ugly black censor boxes; rejected by user.
  2. *Asset modification*: User clarified not to corrupt/delete `.utoc` files on disk.
  3. *Memory manipulation of `WBP_FPV_Drone_C`*: Scanning `GUObjectArray` and setting `RenderOpacity = 0.0f` and `Visibility = 2` (`Hidden`).
- **Challenge**: In UE5, UMG widgets may reset `RenderOpacity` or `Visibility` on each tick if the Blueprint graph executes `SetVisibility` or property bindings in `Tick`.
- **Recommended Next Steps for New Agent**:
  - Option A: Identify the native virtual function `UUserWidget::NativeTick` or Slate draw call `SWidget::Paint` for `WBP_FPV_Drone` and NOP or hook it.
  - Option B: Zero out the Slate brush texture pointer for `A_Aim` and `A_FPV_Battery` inside the widget's UImage components.
  - Option C: Build an IoStore mod archive (`FPVKamikazeDrone-Windows_P.pak` + `.utoc` + `.ucas` in `Content/Paks/~mods/`) containing transparent 1x1 dummy assets for `A_Aim` and `A_FPV_Battery`.

---

## 8. Compilation & Toolchain Guide

### Build Requirements
- **Compiler**: Microsoft Visual Studio MSVC x64 (`cl.exe`)
- **SDK**: Windows 10/11 SDK (Direct2D, DirectWrite, WIC, XInput, PSAPI)

### Compilation Command
Run from `d:\Zona Downloads\FPVKamikazeDrone-1\OSD_Overlay`:
```cmd
call "D:\PROGRAMME\visual studio\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /O2 /Fe:FPV_OSD_Overlay.exe main.cpp
del main.obj
```

### Running & Testing
Use `d:\Zona Downloads\FPVKamikazeDrone-1\Start_Game_With_OSD.bat`:
```bat
@echo off
start "" "%~dp0OSD_Overlay\FPV_OSD_Overlay.exe"
timeout /t 1 /nobreak >nul
start "" "%~dp0FPVKamikazeDrone\Binaries\Win64\FPVKamikazeDrone-Win64-Shipping.exe"
```
Or run `FPV_OSD_Overlay.exe` directly while the game is running. Check `osd_log.txt` for real-time diagnostic output.

---

## 9. Immediate Roadmap for Next Agent

1. **Verify In-Game Appearance**:
   Ensure `FPV_OSD_Overlay.exe` displays cleanly over the Kamikaze drone and stays hidden in menus.
2. **Solidify Recon Drone Detection**:
   Inspect `diag_game.exe` output during live flight of both drones to lock in the exact differentiator (e.g. active pawn class or possessed camera FOV).
3. **Finish In-Game HUD Elimination**:
   Ensure `WBP_FPV_Drone_C` is 100% silenced in memory so no duplicate reticle or white text shines through underneath the new Russian military OSD.
