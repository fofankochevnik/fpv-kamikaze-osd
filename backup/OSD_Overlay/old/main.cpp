#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <xinput.h>
#include <stdio.h>
#include <share.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "xinput.lib")

// Low-res retro OSD canvas resolution (exact match to static template)
const float BASE_WIDTH = 1024.0f;
const float BASE_HEIGHT = 582.0f;

// UE5.5 Game Global Offsets
const uintptr_t RVA_GWORLD = 0xa219d88;
const uintptr_t RVA_GUOBJECTARRAY = 0x9db24e0;
const uintptr_t RVA_GUOBJECTARRAY_COUNT = 0x9db2504;
const uintptr_t RVA_FNAMEPOOL = 0x9fab2c0;

// OSD State
struct OsdTelemetry {
    // Dynamic values
    float pitch;        // degrees, strictly nose pitch (-85 .. +85)
    float altitude;     // meters
    float gas;          // throttle 0..100%
    int flightSeconds;  // timer in seconds
    int rxg;            // -6 or -7
    float txg;          // 0.5 to 1.0
    float currentA;     // Amperes
    int mah;            // mAh capacity
    float vTotal;       // Volts total
    float vCell;        // Volts per cell

    // Internal simulation & seeds
    float baseAmps;
    float maxAmps;
    int baseMah;
    float internalRes;
    float rxgTimer;
    float txgTimer;
    float txgTarget;
    float timerAccum;
    bool isKamikaze;    // True for Kamikaze drone, False ONLY for Recon drone!
    bool memoryHooked;  // True when connected to real game memory
};

// Global application state
HWND g_hwndOverlay = NULL;
DWORD g_gamePid = 0;
HANDLE g_hGameProcess = NULL;
uintptr_t g_gameBase = 0;

ID2D1Factory* g_pD2DFactory = NULL;
ID2D1DCRenderTarget* g_pDCRenderTarget = NULL;
ID2D1BitmapRenderTarget* g_pCanvasRT = NULL;
IDWriteFactory* g_pDWriteFactory = NULL;
IDWriteTextFormat* g_pTextFormat = NULL;
IDWriteTextFormat* g_pTextFormatSmall = NULL;
ID2D1SolidColorBrush* g_pWhiteBrush = NULL;
ID2D1SolidColorBrush* g_pBlackBrush = NULL;
ID2D1Bitmap* g_pTemplateBitmap = NULL;

HDC g_hdcMem = NULL;
HBITMAP g_hBitmap = NULL;
void* g_pBitmapBits = NULL;
int g_curWidth = 0;
int g_curHeight = 0;

bool g_overlayVisible = true;
bool g_running = true;
POINT g_lastMousePos = { 0, 0 };
OsdTelemetry g_telemetry;
FILE* g_pLogFile = NULL;

// Forward declarations
void InitTelemetry();
void UpdateTelemetry(float dt, bool inFlight);
bool UpdateGameTelemetryAndSuppress(float dt);
void RenderOSD(int width, int height);
HWND FindGameWindow();
void RecreateDIB(int width, int height);
bool TryHookGameProcess();
void LogMsg(const char* fmt, ...);

void LogMsg(const char* fmt, ...) {
    if (!g_pLogFile) {
        g_pLogFile = _fsopen("osd_log.txt", "w", _SH_DENYNO);
    }
    if (g_pLogFile) {
        va_list args;
        va_start(args, fmt);
        vfprintf(g_pLogFile, fmt, args);
        va_end(args);
        fflush(g_pLogFile);
    }
}

// Draw text onto the 1024x582 low-res canvas with 4-pass outline
void DrawOsdText(ID2D1RenderTarget* pRT, const wchar_t* text, float x, float y, float w, float h, IDWriteTextFormat* pFormat) {
    if (!pRT || !pFormat || !text) return;

    D2D1_RECT_F rect = D2D1::RectF(x, y, x + w, y + h);

    const float o = 1.0f;
    D2D1_RECT_F rL = D2D1::RectF(rect.left - o, rect.top, rect.right - o, rect.bottom);
    D2D1_RECT_F rR = D2D1::RectF(rect.left + o, rect.top, rect.right + o, rect.bottom);
    D2D1_RECT_F rU = D2D1::RectF(rect.left, rect.top - o, rect.right, rect.bottom - o);
    D2D1_RECT_F rD = D2D1::RectF(rect.left, rect.top + o, rect.right, rect.bottom + o);

    pRT->DrawText(text, (UINT32)wcslen(text), pFormat, rL, g_pBlackBrush);
    pRT->DrawText(text, (UINT32)wcslen(text), pFormat, rR, g_pBlackBrush);
    pRT->DrawText(text, (UINT32)wcslen(text), pFormat, rU, g_pBlackBrush);
    pRT->DrawText(text, (UINT32)wcslen(text), pFormat, rD, g_pBlackBrush);

    pRT->DrawText(text, (UINT32)wcslen(text), pFormat, rect, g_pWhiteBrush);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// Find game window
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == g_gamePid && IsWindowVisible(hwnd)) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        if (rc.right > 400 && rc.bottom > 300) {
            *(HWND*)lParam = hwnd;
            return FALSE;
        }
    }
    return TRUE;
}

HWND FindGameWindow() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD foundPid = 0;
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe;
        pe.dwSize = sizeof(pe);
        if (Process32FirstW(snapshot, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, L"FPVKamikazeDrone-Win64-Shipping.exe") == 0) {
                    foundPid = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &pe));
        }
        CloseHandle(snapshot);
    }

    if (foundPid != 0) {
        g_gamePid = foundPid;
        HWND foundHwnd = NULL;
        EnumWindows(EnumWindowsProc, (LPARAM)&foundHwnd);
        if (foundHwnd) return foundHwnd;

        HWND hwnd = FindWindowW(NULL, L"FPV Kamikaze Drone");
        if (hwnd && IsWindowVisible(hwnd)) return hwnd;
    }

    return NULL;
}

// Hook process memory and find base address
bool TryHookGameProcess() {
    if (g_hGameProcess && g_gameBase != 0) return true;
    if (g_gamePid == 0) return false;

    if (!g_hGameProcess) {
        g_hGameProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, g_gamePid);
    }
    if (!g_hGameProcess) return false;

    if (g_gameBase == 0) {
        HMODULE hMods[1024];
        DWORD cbNeeded = 0;
        if (EnumProcessModules(g_hGameProcess, hMods, sizeof(hMods), &cbNeeded)) {
            g_gameBase = (uintptr_t)hMods[0];
            LogMsg("Game base hooked: 0x%llX (PID %d)\n", g_gameBase, g_gamePid);
        }
    }

    return (g_gameBase != 0);
}

// Resolve FName string from UE5 FNamePool
bool ResolveFName(uint32_t compIndex, char* outName, size_t maxLen) {
    if (!g_hGameProcess || !g_gameBase || !outName || maxLen == 0) return false;
    uint32_t blockIndex = compIndex >> 16;
    uint32_t blockOffset = (compIndex & 0xFFFF) * 2;

    uintptr_t poolAddr = g_gameBase + RVA_FNAMEPOOL;
    uintptr_t blockPtr = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(poolAddr + 0x10 + blockIndex * 8), &blockPtr, sizeof(blockPtr), NULL) || !blockPtr) {
        return false;
    }

    uint16_t header = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(blockPtr + blockOffset), &header, sizeof(header), NULL)) {
        return false;
    }

    bool isWide = (header & 1) != 0;
    uint32_t len = header >> 6;
    if (len == 0 || len >= maxLen) return false;

    if (isWide) {
        wchar_t wideBuf[256];
        size_t readLen = (len < 255) ? len : 255;
        if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(blockPtr + blockOffset + 2), wideBuf, readLen * sizeof(wchar_t), NULL)) {
            wideBuf[readLen] = L'\0';
            WideCharToMultiByte(CP_UTF8, 0, wideBuf, -1, outName, (int)maxLen, NULL, NULL);
            return true;
        }
    } else {
        if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(blockPtr + blockOffset + 2), outName, len, NULL)) {
            outName[len] = '\0';
            return true;
        }
    }
    return false;
}

// Get UObject class name
bool GetUObjectClass(uintptr_t uobj, char* outClassName, size_t maxLen) {
    if (!uobj || !g_hGameProcess) return false;
    uintptr_t classPrivate = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(uobj + 0x10), &classPrivate, sizeof(classPrivate), NULL) || !classPrivate) {
        return false;
    }
    uint32_t nameIndex = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(classPrivate + 0x18), &nameIndex, sizeof(nameIndex), NULL)) {
        return false;
    }
    return ResolveFName(nameIndex, outClassName, maxLen);
}

// Read UObject from GUObjectArray
uintptr_t GetUObject(int32_t index) {
    if (!g_hGameProcess || !g_gameBase || index < 0) return 0;
    int32_t numElements = 0;
    ReadProcessMemory(g_hGameProcess, (LPCVOID)(g_gameBase + RVA_GUOBJECTARRAY_COUNT), &numElements, sizeof(numElements), NULL);
    if (index >= numElements) return 0;

    int chunkIndex = index >> 16;
    int chunkOffset = index & 0xFFFF;

    uintptr_t chunkTable = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(g_gameBase + RVA_GUOBJECTARRAY), &chunkTable, sizeof(chunkTable), NULL) || !chunkTable) {
        return 0;
    }

    uintptr_t chunk = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(chunkTable + chunkIndex * 8), &chunk, sizeof(chunk), NULL) || !chunk) {
        return 0;
    }

    uintptr_t uobj = 0;
    ReadProcessMemory(g_hGameProcess, (LPCVOID)(chunk + chunkOffset * 24), &uobj, sizeof(uobj), NULL);
    return uobj;
}

// Real in-game telemetry & HUD suppression
bool UpdateGameTelemetryAndSuppress(float dt) {
    if (!TryHookGameProcess()) return false;

    // 1. Read GWorld
    uintptr_t uworld = 0;
    if (!ReadProcessMemory(g_hGameProcess, (LPCVOID)(g_gameBase + RVA_GWORLD), &uworld, sizeof(uworld), NULL) || !uworld) {
        // GWorld not yet ready, don't change drone mode
        return false;
    }

    // 2. Trace OwningGameInstance -> LocalPlayers[0] -> PlayerController
    uintptr_t gameInstance = 0;
    uintptr_t giOffsets[] = { 0x1D8, 0x1E0, 0x1E8, 0x1C8, 0x1D0, 0x1B8, 0x1F0 };
    for (int i = 0; i < sizeof(giOffsets)/sizeof(giOffsets[0]); i++) {
        uintptr_t candidate = 0;
        if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(uworld + giOffsets[i]), &candidate, sizeof(candidate), NULL) && candidate > 0x10000) {
            char name[64] = {0};
            if (GetUObjectClass(candidate, name, sizeof(name)) && strstr(name, "GameInstance")) {
                gameInstance = candidate;
                break;
            }
        }
    }

    uintptr_t playerController = 0;
    if (gameInstance) {
        uintptr_t localPlayersData = 0;
        int32_t count = 0;
        if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(gameInstance + 0x38), &localPlayersData, sizeof(localPlayersData), NULL) &&
            ReadProcessMemory(g_hGameProcess, (LPCVOID)(gameInstance + 0x40), &count, sizeof(count), NULL) &&
            count > 0 && localPlayersData) {
            uintptr_t localPlayer = 0;
            ReadProcessMemory(g_hGameProcess, (LPCVOID)localPlayersData, &localPlayer, sizeof(localPlayer), NULL);
            if (localPlayer) {
                ReadProcessMemory(g_hGameProcess, (LPCVOID)(localPlayer + 0x30), &playerController, sizeof(playerController), NULL);
            }
        }
    }

    // 3. Inspect PlayerController: AcknowledgedPawn & CameraManager
    uintptr_t activePawn = 0;
    uintptr_t cameraManager = 0;
    if (playerController) {
        uintptr_t pawnOffsets[] = { 0x338, 0x340, 0x2A8, 0x2B0, 0x330 };
        for (int i = 0; i < sizeof(pawnOffsets)/sizeof(pawnOffsets[0]); i++) {
            uintptr_t candidate = 0;
            if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(playerController + pawnOffsets[i]), &candidate, sizeof(candidate), NULL) && candidate > 0x10000) {
                char name[64] = {0};
                if (GetUObjectClass(candidate, name, sizeof(name)) &&
                    (strstr(name, "Pawn") || strstr(name, "Drone") || strstr(name, "Spectator") || strstr(name, "Striker") || strstr(name, "Kamikaze"))) {
                    activePawn = candidate;
                    break;
                }
            }
        }

        uintptr_t camOffsets[] = { 0x348, 0x350, 0x340, 0x358, 0x328 };
        for (int i = 0; i < sizeof(camOffsets)/sizeof(camOffsets[0]); i++) {
            uintptr_t candidate = 0;
            if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(playerController + camOffsets[i]), &candidate, sizeof(candidate), NULL) && candidate > 0x10000) {
                char name[64] = {0};
                if (GetUObjectClass(candidate, name, sizeof(name)) && strstr(name, "Camera")) {
                    cameraManager = candidate;
                    break;
                }
            }
        }
    }

    // AUTOMATIC DRONE DETECTION:
    // If an active pawn is found, check if it's the Recon drone vs Kamikaze drone
    if (activePawn) {
        char pawnClass[128] = {0};
        GetUObjectClass(activePawn, pawnClass, sizeof(pawnClass));

        if (strstr(pawnClass, "Spectator") || strstr(pawnClass, "Striker") || strstr(pawnClass, "Mavic")) {
            // CONFIRMED RECON DRONE -> HIDE OVERLAY
            g_telemetry.isKamikaze = false;
            return true;
        } else if (strstr(pawnClass, "Kamikaze") || strstr(pawnClass, "Drone_FPV") || strstr(pawnClass, "FPV")) {
            // CONFIRMED KAMIKAZE DRONE -> SHOW OVERLAY
            g_telemetry.isKamikaze = true;
        }
    }

    // 4. READ PHYSICAL CAMERA NOSE PITCH & ALTITUDE
    bool readOrientation = false;
    if (cameraManager) {
        for (uintptr_t off = 0x2100; off < 0x2500; off += 8) {
            double testPitch = 0.0;
            if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(cameraManager + off), &testPitch, sizeof(testPitch), NULL)) {
                if (testPitch >= -89.0 && testPitch <= 89.0 && fabsf((float)testPitch) > 0.05f) {
                    double testYaw = 0, testRoll = 0;
                    ReadProcessMemory(g_hGameProcess, (LPCVOID)(cameraManager + off + 8), &testYaw, sizeof(testYaw), NULL);
                    ReadProcessMemory(g_hGameProcess, (LPCVOID)(cameraManager + off + 16), &testRoll, sizeof(testRoll), NULL);
                    if (testYaw >= -360.0 && testYaw <= 360.0 && testRoll >= -360.0 && testRoll <= 360.0) {
                        g_telemetry.pitch = (float)testPitch;
                        readOrientation = true;

                        double locZ = 0.0;
                        ReadProcessMemory(g_hGameProcess, (LPCVOID)(cameraManager + off - 8), &locZ, sizeof(locZ), NULL);
                        g_telemetry.altitude = (float)(locZ / 100.0);
                        break;
                    }
                }
            }
        }
    }

    if (!readOrientation && activePawn) {
        uintptr_t rootComp = 0;
        ReadProcessMemory(g_hGameProcess, (LPCVOID)(activePawn + 0x198), &rootComp, sizeof(rootComp), NULL);
        if (!rootComp) ReadProcessMemory(g_hGameProcess, (LPCVOID)(activePawn + 0x190), &rootComp, sizeof(rootComp), NULL);
        if (rootComp) {
            for (uintptr_t off = 0x120; off < 0x170; off += 4) {
                float testPitch = 0.0f;
                ReadProcessMemory(g_hGameProcess, (LPCVOID)(rootComp + off), &testPitch, sizeof(testPitch), NULL);
                if (testPitch >= -85.0f && testPitch <= 85.0f && fabsf(testPitch) > 0.05f) {
                    g_telemetry.pitch = testPitch;
                    readOrientation = true;
                    break;
                }
            }
        }
    }

    g_telemetry.memoryHooked = readOrientation;

    // 5. SUPPRESS THE BUILT-IN HUD:
    // Locate WBP_FPV_Drone_C and zero out its RenderOpacity & set Visibility = Hidden
    static int s_suppressCounter = 0;
    s_suppressCounter++;
    if (s_suppressCounter % 20 == 0) {
        int32_t numObjs = 0;
        ReadProcessMemory(g_hGameProcess, (LPCVOID)(g_gameBase + RVA_GUOBJECTARRAY_COUNT), &numObjs, sizeof(numObjs), NULL);
        if (numObjs > 100000) numObjs = 100000;

        for (int32_t i = 0; i < numObjs; i++) {
            uintptr_t obj = GetUObject(i);
            if (!obj) continue;
            char className[64] = {0};
            if (GetUObjectClass(obj, className, sizeof(className))) {
                if (strstr(className, "WBP_FPV_Drone") || strstr(className, "BP_UI_FPV")) {
                    float zero = 0.0f;
                    uint8_t hiddenVis = 2; // Hidden

                    for (uintptr_t off = 0x40; off < 0x180; off += 4) {
                        float curVal = 0.0f;
                        if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(obj + off), &curVal, sizeof(curVal), NULL)) {
                            if (fabsf(curVal - 1.0f) < 0.001f) {
                                WriteProcessMemory(g_hGameProcess, (LPVOID)(obj + off), &zero, sizeof(zero), NULL);
                            }
                        }
                    }
                    for (uintptr_t off = 0x80; off < 0x140; off += 1) {
                        uint8_t curVis = 0;
                        if (ReadProcessMemory(g_hGameProcess, (LPCVOID)(obj + off), &curVis, sizeof(curVis), NULL)) {
                            if (curVis == 0 || curVis == 4) {
                                WriteProcessMemory(g_hGameProcess, (LPVOID)(obj + off), &hiddenVis, sizeof(hiddenVis), NULL);
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

// Load PNG texture with WIC
bool LoadTemplateTexture(const wchar_t* path) {
    IWICImagingFactory* pWICFactory = NULL;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
    if (FAILED(hr)) return false;

    IWICBitmapDecoder* pDecoder = NULL;
    hr = pWICFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
    if (FAILED(hr)) {
        pWICFactory->Release();
        return false;
    }

    IWICBitmapFrameDecode* pFrame = NULL;
    hr = pDecoder->GetFrame(0, &pFrame);
    if (FAILED(hr)) {
        pDecoder->Release();
        pWICFactory->Release();
        return false;
    }

    IWICFormatConverter* pConverter = NULL;
    hr = pWICFactory->CreateFormatConverter(&pConverter);
    if (FAILED(hr)) {
        pFrame->Release();
        pDecoder->Release();
        pWICFactory->Release();
        return false;
    }

    hr = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
    if (SUCCEEDED(hr)) {
        hr = g_pDCRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, &g_pTemplateBitmap);
    }

    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pWICFactory->Release();

    return SUCCEEDED(hr);
}

void RecreateDIB(int width, int height) {
    if (width <= 0 || height <= 0) return;
    if (g_curWidth == width && g_curHeight == height && g_hBitmap != NULL) return;

    if (g_hBitmap) {
        DeleteObject(g_hBitmap);
        g_hBitmap = NULL;
    }
    if (g_hdcMem) {
        DeleteDC(g_hdcMem);
        g_hdcMem = NULL;
    }

    HDC hdcScreen = GetDC(NULL);
    g_hdcMem = CreateCompatibleDC(hdcScreen);
    ReleaseDC(NULL, hdcScreen);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    g_hBitmap = CreateDIBSection(g_hdcMem, &bmi, DIB_RGB_COLORS, &g_pBitmapBits, NULL, 0);
    SelectObject(g_hdcMem, g_hBitmap);

    g_curWidth = width;
    g_curHeight = height;
}

void InitTelemetry() {
    srand((unsigned int)time(NULL));

    g_telemetry.pitch = 0.0f;
    g_telemetry.altitude = 0.0f;
    g_telemetry.gas = 0.0f;
    g_telemetry.flightSeconds = 0;
    g_telemetry.rxg = -6;
    g_telemetry.txg = 1.0f;
    g_telemetry.txgTarget = 1.0f;
    g_telemetry.rxgTimer = (float)(rand() % 10 + 8);
    g_telemetry.txgTimer = 4.0f;
    g_telemetry.timerAccum = 0.0f;
    g_telemetry.isKamikaze = true; // TRUE by default so overlay works immediately upon flying!
    g_telemetry.memoryHooked = false;

    g_telemetry.baseAmps = 1.8f + (float)(rand() % 15) / 10.0f;
    g_telemetry.maxAmps = 32.0f + (float)(rand() % 160) / 10.0f;
    g_telemetry.baseMah = 29500 + (rand() % 2000);
    g_telemetry.mah = g_telemetry.baseMah;
    g_telemetry.internalRes = 0.035f + (float)(rand() % 20) / 1000.0f;
    g_telemetry.vTotal = 22.2f;
    g_telemetry.vCell = 3.70f;
    g_telemetry.currentA = g_telemetry.baseAmps;

    GetCursorPos(&g_lastMousePos);
}

void UpdateTelemetry(float dt, bool inFlight) {
    if (!inFlight) {
        g_telemetry.gas = 0.0f;
        g_telemetry.currentA = g_telemetry.baseAmps;
        return;
    }

    // 1. Flight timer
    g_telemetry.timerAccum += dt;
    if (g_telemetry.timerAccum >= 1.0f) {
        g_telemetry.flightSeconds++;
        g_telemetry.timerAccum -= 1.0f;
    }

    // 2. RXg random fluctuation (-7 .. -6)
    g_telemetry.rxgTimer -= dt;
    if (g_telemetry.rxgTimer <= 0.0f) {
        g_telemetry.rxg = (rand() % 2 == 0) ? -6 : -7;
        g_telemetry.rxgTimer = 8.0f + (float)(rand() % 15);
    }

    // 3. TXg procedural fluctuation (0.5 .. 1.0)
    g_telemetry.txgTimer -= dt;
    if (g_telemetry.txgTimer <= 0.0f) {
        float vals[] = { 1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f };
        g_telemetry.txgTarget = vals[rand() % 6];
        g_telemetry.txgTimer = 5.0f + (float)(rand() % 10);
    }
    if (fabsf(g_telemetry.txg - g_telemetry.txgTarget) > 0.01f) {
        float step = (g_telemetry.txgTarget > g_telemetry.txg) ? 0.08f * dt : -0.08f * dt;
        g_telemetry.txg += step;
    }

    // 4. THROTTLE (ГАЗ) - STRICTLY THROTTLE INPUTS ONLY:
    // Left stick Y (Gamepad/Radio), Right Trigger, Shift, Space
    // NEVER TOUCHED BY W / S (PITCH KEYS)!
    XINPUT_STATE xstate;
    bool hasPad = (XInputGetState(0, &xstate) == ERROR_SUCCESS);

    float userGasInput = 0.0f;

    if (hasPad) {
        SHORT ly = xstate.Gamepad.sThumbLY;
        BYTE rt = xstate.Gamepad.bRightTrigger;
        if (ly > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
            userGasInput = (float)(ly - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        }
        if (rt > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
            float trigGas = (float)rt / 255.0f;
            if (trigGas > userGasInput) userGasInput = trigGas;
        }
    }

    // Keyboard Throttle: Shift / Space increases gas, Ctrl lowers gas
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) userGasInput = max(userGasInput, 0.85f);
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) userGasInput = max(userGasInput, 0.60f);

    float targetGasPercent = userGasInput * 100.0f;
    g_telemetry.gas += (targetGasPercent - g_telemetry.gas) * (6.0f * dt);
    if (g_telemetry.gas < 0.5f) g_telemetry.gas = 0.0f;
    if (g_telemetry.gas > 100.0f) g_telemetry.gas = 100.0f;

    // 5. PITCH (ТАНГАЖ / НАКЛОН НОСА):
    // In FPV drones:
    // W / Up / Mouse Forward / Right Stick Forward = Tilt Nose DOWN (forward flight, pitch decreases)
    // S / Down / Mouse Back / Right Stick Back = Tilt Nose UP (backward flight, pitch increases)
    // PITCH IS COMPLETELY DECOUPLED FROM GAS!
    if (!g_telemetry.memoryHooked) {
        float pitchDelta = 0.0f;

        if (hasPad) {
            SHORT ry = xstate.Gamepad.sThumbRY;
            if (abs(ry) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
                pitchDelta = -(float)ry / 32768.0f;
            }
        }

        POINT curMousePos;
        if (GetCursorPos(&curMousePos)) {
            int dy = curMousePos.y - g_lastMousePos.y;
            if (abs(dy) > 0 && abs(dy) < 200) {
                pitchDelta += (float)dy * 0.15f;
            }
            g_lastMousePos = curMousePos;
        }

        // Keyboard FPV Pitch Controls:
        // W = nose down (-), S = nose up (+)
        if (GetAsyncKeyState('W') & 0x8000) pitchDelta -= 0.7f;
        if (GetAsyncKeyState('S') & 0x8000) pitchDelta += 0.7f;
        if (GetAsyncKeyState(VK_UP) & 0x8000) pitchDelta -= 0.7f;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) pitchDelta += 0.7f;

        g_telemetry.pitch += pitchDelta * (35.0f * dt);
        if (g_telemetry.pitch < -85.0f) g_telemetry.pitch = -85.0f;
        if (g_telemetry.pitch > 85.0f) g_telemetry.pitch = 85.0f;

        // Altitude climbs with throttle
        float climbRate = (g_telemetry.gas - 35.0f) * 0.25f;
        g_telemetry.altitude += climbRate * dt;
    }

    // 6. Dynamic Amperage & mAh calculation
    float throttleNorm = g_telemetry.gas / 100.0f;
    g_telemetry.currentA = g_telemetry.baseAmps + (throttleNorm * throttleNorm) * (g_telemetry.maxAmps - g_telemetry.baseAmps);

    float consumedMahThisFrame = (g_telemetry.currentA * dt / 3600.0f) * 1000.0f;
    g_telemetry.mah += (int)consumedMahThisFrame;

    float sag = g_telemetry.currentA * g_telemetry.internalRes;
    float capacityLoss = ((float)(g_telemetry.mah - g_telemetry.baseMah) / 4000.0f) * 0.4f;
    g_telemetry.vTotal = 22.4f - sag - capacityLoss;
    if (g_telemetry.vTotal < 18.0f) g_telemetry.vTotal = 18.0f;
    g_telemetry.vCell = g_telemetry.vTotal / 6.0f;
}

// Low-Res Render Function:
// 1. 100% transparent background (NO BLACK MASKING BOXES).
// 2. Renders template and numbers onto 1024x582 pixel grid.
// 3. Upscales with NEAREST-NEIGHBOR so all pixels have the exact same resolution!
void RenderOSD(int width, int height) {
    if (!g_pDCRenderTarget || !g_hdcMem || !g_pTemplateBitmap) return;

    RECT rc = { 0, 0, width, height };
    HRESULT hr = g_pDCRenderTarget->BindDC(g_hdcMem, &rc);
    if (FAILED(hr)) return;

    if (!g_pCanvasRT) {
        D2D1_SIZE_F canvasSize = D2D1::SizeF(BASE_WIDTH, BASE_HEIGHT);
        g_pDCRenderTarget->CreateCompatibleRenderTarget(canvasSize, &g_pCanvasRT);
    }
    if (!g_pCanvasRT) return;

    g_pCanvasRT->BeginDraw();
    g_pCanvasRT->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

    // Draw static template at 1:1 on 1024x582
    g_pCanvasRT->DrawBitmap(
        g_pTemplateBitmap,
        D2D1::RectF(0.0f, 0.0f, BASE_WIDTH, BASE_HEIGHT),
        1.0f,
        D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
    );

    // Draw dynamic numbers directly onto the 1024x582 pixel grid
    wchar_t timerBuf[32];
    int mins = g_telemetry.flightSeconds / 60;
    int secs = g_telemetry.flightSeconds % 60;
    swprintf_s(timerBuf, L"%02d:%02d", mins, secs);
    DrawOsdText(g_pCanvasRT, timerBuf, 16.0f, 44.0f, 75.0f, 25.0f, g_pTextFormat);

    wchar_t altBuf[32];
    swprintf_s(altBuf, L"%+.1f", g_telemetry.altitude);
    DrawOsdText(g_pCanvasRT, altBuf, 18.0f, 82.0f, 65.0f, 24.0f, g_pTextFormat);

    wchar_t vTotBuf[32];
    swprintf_s(vTotBuf, L"%.1fV", g_telemetry.vTotal);
    DrawOsdText(g_pCanvasRT, vTotBuf, 430.0f, 22.0f, 65.0f, 24.0f, g_pTextFormat);

    wchar_t vCellBuf[32];
    swprintf_s(vCellBuf, L"%.2fV", g_telemetry.vCell);
    DrawOsdText(g_pCanvasRT, vCellBuf, 565.0f, 22.0f, 65.0f, 24.0f, g_pTextFormat);

    wchar_t pitchBuf[32];
    swprintf_s(pitchBuf, L"%.1f", g_telemetry.pitch);
    DrawOsdText(g_pCanvasRT, pitchBuf, 680.0f, 40.0f, 70.0f, 24.0f, g_pTextFormat);

    wchar_t ampBuf[32];
    swprintf_s(ampBuf, L"%.1f", g_telemetry.currentA);
    DrawOsdText(g_pCanvasRT, ampBuf, 836.0f, 324.0f, 55.0f, 24.0f, g_pTextFormat);

    wchar_t mahBuf[32];
    swprintf_s(mahBuf, L"%d", g_telemetry.mah);
    DrawOsdText(g_pCanvasRT, mahBuf, 836.0f, 357.0f, 75.0f, 24.0f, g_pTextFormat);

    wchar_t rxgBuf[32];
    swprintf_s(rxgBuf, L"%d", g_telemetry.rxg);
    DrawOsdText(g_pCanvasRT, rxgBuf, 78.0f, 410.0f, 45.0f, 24.0f, g_pTextFormat);

    wchar_t gasBuf[32];
    swprintf_s(gasBuf, L"%d", (int)(g_telemetry.gas + 0.5f));
    DrawOsdText(g_pCanvasRT, gasBuf, 892.0f, 410.0f, 35.0f, 24.0f, g_pTextFormat);

    wchar_t txgBuf[32];
    swprintf_s(txgBuf, L"%.1f", g_telemetry.txg);
    DrawOsdText(g_pCanvasRT, txgBuf, 70.0f, 445.0f, 42.0f, 24.0f, g_pTextFormat);

    g_pCanvasRT->EndDraw();

    // Upscale the entire 1024x582 frame together with NEAREST-NEIGHBOR
    ID2D1Bitmap* pCanvasBitmap = NULL;
    g_pCanvasRT->GetBitmap(&pCanvasBitmap);

    if (pCanvasBitmap) {
        g_pDCRenderTarget->BeginDraw();
        g_pDCRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

        g_pDCRenderTarget->DrawBitmap(
            pCanvasBitmap,
            D2D1::RectF(0.0f, 0.0f, (float)width, (float)height),
            1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
        );

        g_pDCRenderTarget->EndDraw();
        pCanvasBitmap->Release();
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    CoInitialize(NULL);
    InitTelemetry();

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"FPV_OSD_Overlay_Class";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassExW(&wc);

    g_hwndOverlay = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"FPV_OSD_Overlay_Class",
        L"FPV Drone OSD Overlay",
        WS_POPUP,
        0, 0, 1024, 582,
        NULL, NULL, hInstance, NULL
    );

    if (!g_hwndOverlay) return 1;

    ShowWindow(g_hwndOverlay, SW_HIDE);

    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
    if (FAILED(hr)) return 1;

    D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        0.0f, 0.0f,
        D2D1_RENDER_TARGET_USAGE_NONE,
        D2D1_FEATURE_LEVEL_DEFAULT
    );
    hr = g_pD2DFactory->CreateDCRenderTarget(&rtProps, &g_pDCRenderTarget);
    if (FAILED(hr)) return 1;

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&g_pDWriteFactory);
    if (SUCCEEDED(hr)) {
        g_pDWriteFactory->CreateTextFormat(
            L"Consolas", NULL,
            DWRITE_FONT_WEIGHT_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            19.0f,
            L"ru-ru",
            &g_pTextFormat
        );
        g_pDWriteFactory->CreateTextFormat(
            L"Consolas", NULL,
            DWRITE_FONT_WEIGHT_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            16.0f,
            L"ru-ru",
            &g_pTextFormatSmall
        );
    }

    g_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.95f, 0.95f, 0.95f, 1.0f), &g_pWhiteBrush);
    g_pDCRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.92f), &g_pBlackBrush);

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    wchar_t* lastSlash = wcsrchr(exePath, L'\\');
    if (lastSlash) *(lastSlash + 1) = L'\0';
    wchar_t templatePath[MAX_PATH];
    swprintf_s(templatePath, L"%sosd_template.png", exePath);

    if (!LoadTemplateTexture(templatePath)) {
        if (!LoadTemplateTexture(L"osd_template.png")) {
            return 1;
        }
    }

    RecreateDIB(1024, 582);

    DWORD lastTick = GetTickCount();
    MSG msg = {};

    RegisterHotKey(g_hwndOverlay, 1, 0, VK_F6); // Optional toggle overlay visibility
    RegisterHotKey(g_hwndOverlay, 2, 0, VK_F7); // Reset timer

    bool gameEverSeen = false;

    while (g_running) {
        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                g_running = false;
                break;
            }
            if (msg.message == WM_HOTKEY) {
                if (msg.wParam == 1) { // F6
                    g_overlayVisible = !g_overlayVisible;
                } else if (msg.wParam == 2) { // F7
                    g_telemetry.flightSeconds = 0;
                    g_telemetry.mah = g_telemetry.baseMah;
                }
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        DWORD now = GetTickCount();
        float dt = (float)(now - lastTick) / 1000.0f;
        if (dt <= 0.0f) dt = 0.001f;
        if (dt > 0.1f) dt = 0.1f;
        lastTick = now;

        // Auto-close overlay when game closes
        if (g_gamePid != 0 && g_hGameProcess != NULL) {
            DWORD exitCode = 0;
            if (GetExitCodeProcess(g_hGameProcess, &exitCode) && exitCode != STILL_ACTIVE) {
                ExitProcess(0);
            }
        }

        HWND hGame = FindGameWindow();
        if (hGame) {
            gameEverSeen = true;
        } else {
            if (gameEverSeen) {
                ExitProcess(0);
            }
            ShowWindow(g_hwndOverlay, SW_HIDE);
            Sleep(250);
            continue;
        }

        if (!IsWindowVisible(hGame) || IsIconic(hGame)) {
            ShowWindow(g_hwndOverlay, SW_HIDE);
            Sleep(50);
            continue;
        }

        // Hide when in menus (cursor is visible)
        CURSORINFO ci = { sizeof(ci) };
        bool cursorShowing = true;
        if (GetCursorInfo(&ci)) {
            cursorShowing = (ci.flags & CURSOR_SHOWING) != 0;
        }

        HWND foreground = GetForegroundWindow();
        bool gameIsForeground = (foreground == hGame);

        // Update real in-game memory telemetry & auto-detect drone type
        UpdateGameTelemetryAndSuppress(dt);

        // ACTIVE FLIGHT DECISION:
        // 1. Must be in flight (cursor is hidden by game)
        // 2. Must be the Kamikaze drone (NOT Recon/Spectator/Mavic)
        // 3. Game must be in foreground
        // 4. Overlay not manually toggled off
        bool inFlight = (!cursorShowing && gameIsForeground);
        bool shouldShow = inFlight && g_telemetry.isKamikaze && g_overlayVisible;

        if (!shouldShow) {
            ShowWindow(g_hwndOverlay, SW_HIDE);
            Sleep(30);
            continue;
        }

        // Track game client area
        POINT ptClient = { 0, 0 };
        ClientToScreen(hGame, &ptClient);
        RECT rcClient;
        GetClientRect(hGame, &rcClient);

        int targetWidth = rcClient.right;
        int targetHeight = rcClient.bottom;

        if (targetWidth > 100 && targetHeight > 100) {
            RecreateDIB(targetWidth, targetHeight);
            UpdateTelemetry(dt, shouldShow);
            RenderOSD(targetWidth, targetHeight);

            HDC hdcScreen = GetDC(NULL);
            POINT ptSrc = { 0, 0 };
            POINT ptDst = { ptClient.x, ptClient.y };
            SIZE size = { targetWidth, targetHeight };
            BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

            UpdateLayeredWindow(g_hwndOverlay, hdcScreen, &ptDst, &size, g_hdcMem, &ptSrc, 0, &blend, ULW_ALPHA);
            ReleaseDC(NULL, hdcScreen);

            ShowWindow(g_hwndOverlay, SW_SHOWNOACTIVATE);
            SetWindowPos(g_hwndOverlay, HWND_TOPMOST, ptClient.x, ptClient.y, targetWidth, targetHeight,
                SWP_NOACTIVATE | SWP_SHOWWINDOW);
        }

        Sleep(16); // ~60 FPS
    }

    if (g_pCanvasRT) g_pCanvasRT->Release();
    if (g_hGameProcess) CloseHandle(g_hGameProcess);
    if (g_pLogFile) fclose(g_pLogFile);
    CoUninitialize();
    return 0;
}
