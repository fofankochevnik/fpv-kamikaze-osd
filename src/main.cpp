typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;
typedef short i16;
typedef unsigned long long u64;
typedef long long i64;
typedef wchar_t wc;
typedef void* HANDLE;
typedef HANDLE HWND;
typedef HANDLE HDC;
typedef HANDLE HINSTANCE;

struct POINT { i32 x, y; };
struct RECT { i32 l, t, r, b; };
struct SIZE { i32 cx, cy; };
struct MSG { HWND hwnd; u32 message; u64 wParam; i64 lParam; u32 time; POINT pt; u32 priv; };
struct WNDCLASSEXW { u32 cbSize, style; i64 (*proc)(HWND, u32, u64, i64); i32 clsExtra, wndExtra; HINSTANCE inst; HANDLE icon, cursor, bg; const wc* menu; const wc* cls; HANDLE iconSm; };
struct CURSORINFO { u32 cbSize, flags; HANDLE cursor; POINT pt; };
struct BITMAPINFOHEADER { u32 size; i32 w, h; u16 planes, bits; u32 comp, sizeImage; i32 xppm, yppm; u32 used, important; };
struct BITMAPINFO { BITMAPINFOHEADER h; u32 colors[1]; };
struct BLENDFUNCTION { u8 op, flags, alpha, fmt; };
struct PROCESSENTRY32W { u32 dwSize, cntUsage, pid; u64 heap; u32 mod, threads, parent; i32 pri; u32 flags; wc exe[260]; };
struct SYSTEMTIME { u16 y, mo, dow, d, h, mi, s, ms; };
struct XINPUT_STATE { u32 pkt; u16 buttons; u8 lt, rt; i16 lx, ly, rx, ry; };
struct GdiplusStartupInput { u32 ver; void* cb; i32 noBg; i32 noCodecs; };
struct RectF { float x, y, w, h; };
struct CRITICAL_SECTION { u64 b[5]; };

#define IMP extern "C" __declspec(dllimport)
IMP HANDLE __stdcall CreateToolhelp32Snapshot(u32, u32);
IMP i32 __stdcall Process32FirstW(HANDLE, PROCESSENTRY32W*);
IMP i32 __stdcall Process32NextW(HANDLE, PROCESSENTRY32W*);
IMP i32 __stdcall CloseHandle(HANDLE);
IMP HANDLE __stdcall OpenProcess(u32, i32, u32);
IMP i32 __stdcall ReadProcessMemory(HANDLE, const void*, void*, u64, u64*);
IMP i32 __stdcall WriteProcessMemory(HANDLE, void*, const void*, u64, u64*);
IMP i32 __stdcall GetExitCodeProcess(HANDLE, u32*);
IMP i32 __stdcall K32EnumProcessModules(HANDLE, void**, u32, u32*);
IMP void __stdcall Sleep(u32);
IMP HANDLE __stdcall CreateEventW(void*, i32, i32, const wc*);
IMP i32 __stdcall SetEvent(HANDLE);
IMP u32 __stdcall WaitForSingleObject(HANDLE, u32);
IMP void __stdcall ExitProcess(u32);
IMP i32 __stdcall QueryPerformanceCounter(i64*);
IMP i32 __stdcall QueryPerformanceFrequency(i64*);
IMP u64 __stdcall GetTickCount64();
IMP u32 __stdcall GetModuleFileNameW(void*, wc*, u32);
IMP void* __stdcall GetModuleHandleW(const wc*);
IMP HANDLE __stdcall CreateFileW(const wc*, u32, u32, void*, u32, u32, HANDLE);
IMP i32 __stdcall WriteFile(HANDLE, const void*, u32, u32*, void*);
IMP HANDLE __stdcall CreateThread(void*, u64, u32 (__stdcall*)(void*), void*, u32, u32*);
IMP u32 __stdcall GetPrivateProfileStringW(const wc*, const wc*, const wc*, wc*, u32, const wc*);
IMP u32 __stdcall GetPrivateProfileSectionW(const wc*, wc*, u32, const wc*);
IMP void* __stdcall VirtualAlloc(void*, u64, u32, u32);
IMP i32 __stdcall VirtualFree(void*, u64, u32);
IMP HANDLE __stdcall CreateMutexW(void*, i32, const wc*);
IMP u32 __stdcall GetLastError();
IMP void __stdcall InitializeCriticalSection(CRITICAL_SECTION*);
IMP void __stdcall EnterCriticalSection(CRITICAL_SECTION*);
IMP void __stdcall LeaveCriticalSection(CRITICAL_SECTION*);
IMP void __stdcall GetLocalTime(SYSTEMTIME*);
IMP void* __stdcall LoadLibraryW(const wc*);
IMP void* __stdcall GetProcAddress(void*, const char*);
IMP u32 __stdcall GetFileAttributesW(const wc*);
struct MBI { u64 base, allocBase; u32 allocProt; u16 part; u16 pad0; u64 size; u32 state, prot, type, pad1; };
IMP u64 __stdcall VirtualQueryEx(HANDLE, const void*, MBI*, u64);

IMP u16 __stdcall RegisterClassExW(const WNDCLASSEXW*);
IMP HWND __stdcall CreateWindowExW(u32, const wc*, const wc*, u32, i32, i32, i32, i32, HWND, void*, void*, void*);
IMP i64 __stdcall DefWindowProcW(HWND, u32, u64, i64);
IMP i32 __stdcall PeekMessageW(MSG*, HWND, u32, u32, u32);
IMP i32 __stdcall TranslateMessage(const MSG*);
IMP i64 __stdcall DispatchMessageW(const MSG*);
IMP void __stdcall PostQuitMessage(i32);
IMP i32 __stdcall ShowWindow(HWND, i32);
IMP i32 __stdcall SetWindowPos(HWND, HWND, i32, i32, i32, i32, u32);
IMP i32 __stdcall UpdateLayeredWindow(HWND, HDC, POINT*, SIZE*, HDC, POINT*, u32, BLENDFUNCTION*, u32);
IMP HDC __stdcall GetDC(HWND);
IMP i32 __stdcall ReleaseDC(HWND, HDC);
IMP i32 __stdcall EnumWindows(i32 (__stdcall*)(HWND, i64), i64);
IMP u32 __stdcall GetWindowThreadProcessId(HWND, u32*);
IMP i32 __stdcall IsWindowVisible(HWND);
IMP i32 __stdcall IsIconic(HWND);
IMP i32 __stdcall IsWindow(HWND);
IMP i32 __stdcall GetClientRect(HWND, RECT*);
IMP i32 __stdcall ClientToScreen(HWND, POINT*);
IMP HWND __stdcall GetForegroundWindow();
IMP i32 __stdcall GetCursorInfo(CURSORINFO*);
IMP i16 __stdcall GetAsyncKeyState(i32);
IMP i32 __stdcall RegisterHotKey(HWND, i32, u32, u32);
IMP i32 __stdcall MessageBoxW(HWND, const wc*, const wc*, u32);

IMP HDC __stdcall CreateCompatibleDC(HDC);
IMP HANDLE __stdcall CreateDIBSection(HDC, const BITMAPINFO*, u32, void**, HANDLE, u32);
IMP HANDLE __stdcall SelectObject(HDC, HANDLE);
IMP i32 __stdcall DeleteObject(HANDLE);
IMP i32 __stdcall DeleteDC(HDC);
IMP i32 __stdcall BitBlt(HDC, i32, i32, i32, i32, HDC, i32, i32, u32);

IMP i32 __stdcall GdiplusStartup(u64*, const GdiplusStartupInput*, void*);
IMP i32 __stdcall GdipCreateBitmapFromFile(const wc*, void**);
IMP i32 __stdcall GdipCreateBitmapFromScan0(i32, i32, i32, i32, u8*, void**);
IMP i32 __stdcall GdipGetImageGraphicsContext(void*, void**);
IMP i32 __stdcall GdipGraphicsClear(void*, u32);
IMP i32 __stdcall GdipSetInterpolationMode(void*, i32);
IMP i32 __stdcall GdipSetPixelOffsetMode(void*, i32);
IMP i32 __stdcall GdipSetTextRenderingHint(void*, i32);
IMP i32 __stdcall GdipSetSmoothingMode(void*, i32);
IMP i32 __stdcall GdipDrawImageRectI(void*, void*, i32, i32, i32, i32);
IMP i32 __stdcall GdipCreateFontFamilyFromName(const wc*, void*, void**);
IMP i32 __stdcall GdipGetGenericFontFamilySerif(void**);
IMP i32 __stdcall GdipCreateFont(void*, float, i32, i32, void**);
IMP i32 __stdcall GdipCreateSolidFill(u32, void**);
IMP i32 __stdcall GdipCreateStringFormat(i32, u16, void**);
IMP i32 __stdcall GdipSetStringFormatAlign(void*, i32);
IMP i32 __stdcall GdipDrawString(void*, const wc*, i32, void*, const RectF*, void*, void*);
IMP i32 __stdcall GdipFillRectangleI(void*, void*, i32, i32, i32, i32);
IMP i32 __stdcall GdipDeleteGraphics(void*);
IMP i32 __stdcall GdipDisposeImage(void*);
IMP i32 __stdcall GdipCreatePen1(u32, float, i32, void**);
IMP i32 __stdcall GdipSetPenLineCap197819(void*, i32, i32, i32);
IMP i32 __stdcall GdipDrawLine(void*, void*, float, float, float, float);

extern "C" i32 _fltused = 0;
#if defined(_MSC_VER) && !defined(__clang__)
#pragma function(memset, memcpy)
#endif
extern "C" void* memset(void* d, i32 c, u64 n) { volatile u8* p = (u8*)d; while (n--) *p++ = (u8)c; return d; }
extern "C" void* memcpy(void* d, const void* s, u64 n) { volatile u8* p = (u8*)d; const u8* q = (const u8*)s; while (n--) *p++ = *q++; return d; }

static const u64 RVA_GWORLD = 0xa1ffd88;
static const u64 RVA_OBJECTS = 0x9d984e0;
static const u64 RVA_NUMOBJ = 0x9d984f4;
static const u64 RVA_NAMEPOOL = 0x9f912c0;
static const i32 CW = 1024, CH = 582;

static i32 slen(const char* s) { i32 n = 0; while (s[n]) n++; return n; }
static i32 wlen(const wc* s) { i32 n = 0; while (s[n]) n++; return n; }
static char lc(char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }
static bool seq(const char* a, const char* b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static bool sieq(const char* a, const char* b) { while (*a && lc(*a) == lc(*b)) { a++; b++; } return *a == *b; }
static bool sstarts(const char* a, const char* p) { while (*p) { if (*a++ != *p++) return false; } return true; }
static bool sends(const char* a, const char* p) { i32 la = slen(a), lp = slen(p); return la >= lp && seq(a + la - lp, p); }
static bool icontains(const char* h, const char* n) {
    if (!*n) return false;
    for (; *h; h++) { const char* a = h; const char* b = n; while (*a && *b && lc(*a) == lc(*b)) { a++; b++; } if (!*b) return true; }
    return false;
}
static void scpy(char* d, const char* s, i32 max) { i32 i = 0; while (s[i] && i < max - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static void wcpy(wc* d, const wc* s, i32 max) { i32 i = 0; while (s[i] && i < max - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static void wcat(wc* d, const wc* s, i32 max) { i32 n = wlen(d); wcpy(d + n, s, max - n); }

struct SB {
    char* p; u32 n, cap;
    void c(char ch) { if (n + 1 < cap) p[n++] = ch; }
    void s(const char* x) { while (*x) c(*x++); }
    void i(i64 v) { char t[24]; i32 k = 0; bool neg = v < 0; u64 u = neg ? (u64)0 - (u64)v : (u64)v; do { t[k++] = '0' + (char)(u % 10); u /= 10; } while (u); if (neg) c('-'); while (k) c(t[--k]); }
    void h(u64 v) { s("0x"); char t[20]; i32 k = 0; do { i32 d = (i32)(v & 15); t[k++] = d < 10 ? '0' + d : 'A' + d - 10; v >>= 4; } while (v); while (k) c(t[--k]); }
    void f(double v, i32 dec) {
        if (v != v) { s("nan"); return; }
        if (v > 1e15 || v < -1e15) { s(v < 0 ? "-big" : "big"); return; }
        if (v < 0) { c('-'); v = -v; }
        double m = 1; for (i32 k = 0; k < dec; k++) m *= 10;
        i64 t = (i64)(v * m + 0.5); i64 ip = t / (i64)m; i64 fp = t % (i64)m;
        i(ip);
        if (dec > 0) { c('.'); char b[20]; for (i32 k = dec - 1; k >= 0; k--) { b[k] = '0' + (char)(fp % 10); fp /= 10; } for (i32 k = 0; k < dec; k++) c(b[k]); }
    }
    void z() { p[n < cap ? n : cap - 1] = 0; }
};

static wc g_dir[520];
static wc g_logPath[600], g_dumpPath[600], g_cfgPath[600];
static CRITICAL_SECTION g_logCs, g_nameCs;

static void AppendFile(const wc* path, const char* data, u32 len) {
    HANDLE f = CreateFileW(path, 4, 3, 0, 4, 0x80, 0);
    if (f == (HANDLE)-1) return;
    u32 w = 0; WriteFile(f, data, len, &w, 0); CloseHandle(f);
}
static void TruncFile(const wc* path) {
    HANDLE f = CreateFileW(path, 0x40000000, 3, 0, 2, 0x80, 0);
    if (f != (HANDLE)-1) CloseHandle(f);
}
static char g_logBuf[4096];
static void Log(const char* a, const char* b = 0, const char* c = 0, const char* d = 0) {
    EnterCriticalSection(&g_logCs);
    SB sb{ g_logBuf, 0, sizeof(g_logBuf) };
    SYSTEMTIME t; GetLocalTime(&t);
    if (t.h < 10) sb.c('0'); sb.i(t.h); sb.c(':'); if (t.mi < 10) sb.c('0'); sb.i(t.mi); sb.c(':'); if (t.s < 10) sb.c('0'); sb.i(t.s); sb.s("  ");
    sb.s(a); if (b) sb.s(b); if (c) sb.s(c); if (d) sb.s(d); sb.s("\r\n");
    AppendFile(g_logPath, sb.p, sb.n);
    LeaveCriticalSection(&g_logCs);
}

struct Config {
    i32 startTimer, cells;
    double mahStart, packMah, ampsIdleMin, ampsIdleMax, ampsMaxMin, ampsMaxMax;
    wc font[64]; double fontSize; i32 fontBold;
    i32 invertPitch, altRelative, useTemplate;
    char kamikazeMatch[256], reconMatch[256];
    i32 fallbackShow;
    char throttleProp[64], batteryProp[64];
    i32 hideHud, hideImages, hideText, fullCam, keepWrecks, noNoise, camHeight, lodIgnoreFov; double knockScale, folCull, folMin, folFade, frzTime, noImgTime, frzSpeed, frzBack, grayA; i32 impactFx;
    char hudWidgets[256], keepWidgets[512];
    i32 keyToggle, keyReset, keyDump, keyThrUp, keyThrDown;
} C;

static i64 ParseInt(const wc* s, i64 def) {
    while (*s == ' ') s++;
    if (!*s) return def;
    bool neg = false; if (*s == '-') { neg = true; s++; }
    i64 v = 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
        for (;; s++) { i32 d; if (*s >= '0' && *s <= '9') d = *s - '0'; else if (*s >= 'a' && *s <= 'f') d = *s - 'a' + 10; else if (*s >= 'A' && *s <= 'F') d = *s - 'A' + 10; else break; v = v * 16 + d; }
    } else {
        if (*s < '0' || *s > '9') return def;
        for (; *s >= '0' && *s <= '9'; s++) v = v * 10 + (*s - '0');
    }
    return neg ? -v : v;
}
static double ParseDbl(const wc* s, double def) {
    while (*s == ' ') s++;
    if (!*s) return def;
    bool neg = false; if (*s == '-') { neg = true; s++; }
    if ((*s < '0' || *s > '9') && *s != '.') return def;
    double v = 0; for (; *s >= '0' && *s <= '9'; s++) v = v * 10 + (*s - '0');
    if (*s == '.' || *s == ',') { s++; double m = 0.1; for (; *s >= '0' && *s <= '9'; s++) { v += (*s - '0') * m; m *= 0.1; } }
    return neg ? -v : v;
}
static wc g_tmpW[600];
static const wc* Ini(const wc* sec, const wc* key, const wc* def) {
    GetPrivateProfileStringW(sec, key, def, g_tmpW, 600, g_cfgPath);
    return g_tmpW;
}
static i32 IniI(const wc* sec, const wc* key, i64 def) { return (i32)ParseInt(Ini(sec, key, L""), def); }
static double IniD(const wc* sec, const wc* key, double def) { return ParseDbl(Ini(sec, key, L""), def); }
static void IniA(const wc* sec, const wc* key, const wc* def, char* out, i32 max) {
    const wc* v = Ini(sec, key, def); i32 i = 0; while (v[i] && i < max - 1) { out[i] = (char)(v[i] < 128 ? v[i] : '?'); i++; } out[i] = 0;
}

static const char* DEFAULT_CFG =
    "; FPV OSD overlay config\r\n"
    "[OSD]\r\n"
    "StartTimer=00:00\r\n"
    "MahStart=0\r\n"
    "PackMah=5000\r\n"
    "Cells=6\r\n"
    "IdleAmpsMin=1.8\r\n"
    "IdleAmpsMax=3.2\r\n"
    "MaxAmpsMin=32\r\n"
    "MaxAmpsMax=48\r\n"
    "FontName=Georgia\r\n"
    "FontSize=19\r\n"
    "FontBold=1\r\n"
    "InvertPitch=0\r\n"
    "AltitudeRelative=0\r\n"
    "UseTemplateImage=0\r\n"
    "\r\n"
    "[Detect]\r\n"
    "KamikazeMatch=Kamikaze\r\n"
    "ReconMatch=Striker,Spectator,Mavic\r\n"
    "FallbackShow=1\r\n"
    "\r\n"
    "[Telemetry]\r\n"
    "ThrottleProp=\r\n"
    "BatteryProp=\r\n"
    "ThrottleUpKey=0\r\n"
    "ThrottleDownKey=0\r\n"
    "\r\n"
    "[Hud]\r\n"
    "HideGameHud=1\r\n"
    "FullscreenCamera=1\r\n"
    "KeepWrecks=1\r\n"
    "NoInterference=1\r\n"
    "CameraHeight=720\r\n"
    "LodIgnoreFov=1\r\n"
    "KnockbackScale=0.0\r\n"
    "FoliageCullScale=1.0\r\n"
    "FoliageMinCullDistance=40000\r\n"
    "FoliageFadeStart=0.7\r\n"
    "HideImages=1\r\n"
    "HideText=1\r\n"
    "HudWidgets=WBP_FPV_Drone_C\r\n"
    "KeepWidgets=\r\n"
    "\r\n"
    "[Impact]\r\n"
    "Enable=1\r\n"
    "FreezeSeconds=0.7\r\n"
    "NoImageSeconds=3.0\r\n"
    "GrayAlpha=0.2\r\n"
    "LookbackMs=0\r\n"
    "MinSpeed=0\r\n"
    "\r\n"
    "[Hotkeys]\r\n"
    "Toggle=0x75\r\n"
    "ResetTimer=0x76\r\n"
    "Dump=0x78\r\n";

static void LoadConfig() {
    if (GetFileAttributesW(g_cfgPath) == 0xFFFFFFFF) AppendFile(g_cfgPath, DEFAULT_CFG, slen(DEFAULT_CFG));
    const wc* t = Ini(L"OSD", L"StartTimer", L"00:00");
    i32 mm = 0, ss = 0; const wc* p = t; while (*p >= '0' && *p <= '9') mm = mm * 10 + (*p++ - '0'); if (*p == ':') { p++; while (*p >= '0' && *p <= '9') ss = ss * 10 + (*p++ - '0'); }
    C.startTimer = mm * 60 + ss;
    C.mahStart = IniD(L"OSD", L"MahStart", 0);
    C.packMah = IniD(L"OSD", L"PackMah", 5000);
    C.cells = IniI(L"OSD", L"Cells", 6); if (C.cells < 1 || C.cells > 14) C.cells = 6;
    C.ampsIdleMin = IniD(L"OSD", L"IdleAmpsMin", 1.8); C.ampsIdleMax = IniD(L"OSD", L"IdleAmpsMax", 3.2);
    C.ampsMaxMin = IniD(L"OSD", L"MaxAmpsMin", 32); C.ampsMaxMax = IniD(L"OSD", L"MaxAmpsMax", 48);
    wcpy(C.font, Ini(L"OSD", L"FontName", L"Georgia"), 64);
    C.fontSize = IniD(L"OSD", L"FontSize", 19); C.fontBold = IniI(L"OSD", L"FontBold", 1);
    C.invertPitch = IniI(L"OSD", L"InvertPitch", 0); C.useTemplate = IniI(L"OSD", L"UseTemplateImage", 0); C.altRelative = IniI(L"OSD", L"AltitudeRelative", 0);
    IniA(L"Detect", L"KamikazeMatch", L"Kamikaze", C.kamikazeMatch, 256);
    IniA(L"Detect", L"ReconMatch", L"Striker,Spectator,Mavic", C.reconMatch, 256);
    C.fallbackShow = IniI(L"Detect", L"FallbackShow", 1);
    IniA(L"Telemetry", L"ThrottleProp", L"", C.throttleProp, 64);
    IniA(L"Telemetry", L"BatteryProp", L"", C.batteryProp, 64);
    C.keyThrUp = IniI(L"Telemetry", L"ThrottleUpKey", 0); C.keyThrDown = IniI(L"Telemetry", L"ThrottleDownKey", 0);
    C.hideHud = IniI(L"Hud", L"HideGameHud", 1); C.fullCam = IniI(L"Hud", L"FullscreenCamera", 1); C.keepWrecks = IniI(L"Hud", L"KeepWrecks", 1); C.noNoise = IniI(L"Hud", L"NoInterference", 1); C.camHeight = IniI(L"Hud", L"CameraHeight", 720); C.lodIgnoreFov = IniI(L"Hud", L"LodIgnoreFov", 1); C.knockScale = IniD(L"Hud", L"KnockbackScale", 0.0); C.folCull = IniD(L"Hud", L"FoliageCullScale", 1.0); C.folMin = IniD(L"Hud", L"FoliageMinCullDistance", 40000); C.folFade = IniD(L"Hud", L"FoliageFadeStart", 0.7);
    C.impactFx = IniI(L"Impact", L"Enable", 1); C.frzTime = IniD(L"Impact", L"FreezeSeconds", 0.7); C.noImgTime = IniD(L"Impact", L"NoImageSeconds", 3.0);
    C.grayA = IniD(L"Impact", L"GrayAlpha", 0.2); C.frzBack = IniD(L"Impact", L"LookbackMs", 0); C.frzSpeed = IniD(L"Impact", L"MinSpeed", 0); C.hideImages = IniI(L"Hud", L"HideImages", 1); C.hideText = IniI(L"Hud", L"HideText", 1);
    IniA(L"Hud", L"HudWidgets", L"WBP_FPV_Drone_C", C.hudWidgets, 256);
    IniA(L"Hud", L"KeepWidgets", L"", C.keepWidgets, 512);
    C.keyToggle = IniI(L"Hotkeys", L"Toggle", 0x75); C.keyReset = IniI(L"Hotkeys", L"ResetTimer", 0x76); C.keyDump = IniI(L"Hotkeys", L"Dump", 0x78);
}

static bool ListMatch(const char* list, const char* name, bool contains) {
    char tok[96]; i32 k = 0;
    for (const char* p = list;; p++) {
        if (*p == ',' || *p == 0) {
            tok[k] = 0; i32 a = 0; while (tok[a] == ' ') a++; i32 e = k; while (e > a && tok[e - 1] == ' ') tok[--e] = 0;
            if (tok[a]) { if (contains ? icontains(name, tok + a) : sieq(name, tok + a)) return true; }
            k = 0; if (!*p) break;
        } else if (k < 95) tok[k++] = *p;
    }
    return false;
}

static HANDLE g_proc = 0;
static u32 g_pid = 0;
static u64 g_base = 0, g_imgSize = 0;

static bool RPM(u64 a, void* b, u64 n) { if (!g_proc || a < 0x10000 || a > 0x7FFFFFFFFFFFull) return false; u64 r = 0; return ReadProcessMemory(g_proc, (void*)a, b, n, &r) && r == n; }
template <class T> static T Rd(u64 a) { T v; memset(&v, 0, sizeof(v)); RPM(a, &v, sizeof(T)); return v; }
static bool WPM(u64 a, const void* b, u64 n) { if (!g_proc) return false; u64 w = 0; return WriteProcessMemory(g_proc, (void*)a, b, n, &w) && w == n; }
static bool IsPtr(u64 p) { return p >= 0x10000 && p < 0x7FFFFFFFFFFFull && (p & 7) == 0; }
static bool InModule(u64 p) { return p >= g_base && p < g_base + g_imgSize; }

static u8* g_nb[8192];
static u64 g_nbTick[8192];
static bool NameIdx(u32 idx, u32 num, char* out, i32 max) {
    out[0] = 0;
    u32 b = idx >> 16, off = (idx & 0xFFFF) * 2;
    if (b >= 8192 || max < 2) return false;
    bool ok = false;
    EnterCriticalSection(&g_nameCs);
    for (i32 att = 0; att < 2 && !ok; att++) {
        if (!g_nb[b] || att == 1) {
            u64 now = GetTickCount64();
            if (att == 1 && now - g_nbTick[b] < 150) break;
            g_nbTick[b] = now;
            u64 bp = Rd<u64>(g_base + RVA_NAMEPOOL + 0x10 + (u64)b * 8);
            if (!IsPtr(bp & ~7ull) || !bp) break;
            if (!g_nb[b]) g_nb[b] = (u8*)VirtualAlloc(0, 0x20000, 0x3000, 4);
            if (!g_nb[b]) break;
            if (!RPM(bp, g_nb[b], 0x20000)) { for (u32 pg = 0; pg < 0x20000; pg += 0x1000) RPM(bp + pg, g_nb[b] + pg, 0x1000); }
        }
        u16 hdr = *(u16*)(g_nb[b] + off);
        u32 len = hdr >> 6; bool wide = hdr & 1;
        if (len == 0 || len > 1023 || off + 2 + len * (wide ? 2 : 1) > 0x20000) continue;
        i32 n = 0;
        for (u32 i = 0; i < len && n < max - 1; i++) {
            u32 ch = wide ? ((u16*)(g_nb[b] + off + 2))[i] : g_nb[b][off + 2 + i];
            out[n++] = (char)(ch < 128 ? ch : '?');
        }
        out[n] = 0; ok = true;
    }
    LeaveCriticalSection(&g_nameCs);
    if (ok && num > 0) {
        i32 n = slen(out); char t[16]; i32 k = 0; u32 v = num - 1; do { t[k++] = '0' + v % 10; v /= 10; } while (v);
        if (n + k + 2 < max) { out[n++] = '_'; while (k) out[n++] = t[--k]; out[n] = 0; }
    }
    return ok;
}
static bool ObjName(u64 obj, char* out, i32 max) {
    out[0] = 0; if (!IsPtr(obj)) return false;
    u32 nm[2]; if (!RPM(obj + 0x18, nm, 8)) return false;
    return NameIdx(nm[0], nm[1], out, max);
}
static u64 ObjClass(u64 obj) { return IsPtr(obj) ? Rd<u64>(obj + 0x10) : 0; }
static bool ClassNameOf(u64 obj, char* out, i32 max) { return ObjName(ObjClass(obj), out, max); }

struct Layout { u32 sup, child, psize, fNext, fName, pElem, pOff, pExtra, arrInner; bool ok; } L = { 0x40, 0x50, 0x58, 0x20, 0x28, 0, 0, 0, 0, false };

static u64 Super(u64 s) { return Rd<u64>(s + L.sup); }
static u64 FirstField(u64 s) { return Rd<u64>(s + L.child); }
static u64 NextField(u64 f) { return Rd<u64>(f + L.fNext); }
static i32 PropsSize(u64 s) { return Rd<i32>(s + L.psize); }
static bool FieldName(u64 f, char* out, i32 max) { u32 nm[2]; if (!RPM(f + L.fName, nm, 8)) { out[0] = 0; return false; } return NameIdx(nm[0], nm[1], out, max); }
static bool FieldType(u64 f, char* out, i32 max) { u64 fc = Rd<u64>(f + 8); u32 nm = Rd<u32>(fc); out[0] = 0; return IsPtr(fc) && NameIdx(nm, 0, out, max); }
static i32 FieldOff(u64 f) { return Rd<i32>(f + L.pOff); }
static i32 FieldElem(u64 f) { return Rd<i32>(f + L.pElem); }
static u64 FieldExtra(u64 f) { return Rd<u64>(f + L.pExtra); }

static u64 FindField(u64 strct, const char* name, bool supers) {
    char nm[128];
    for (i32 d = 0; d < 32 && IsPtr(strct); d++) {
        u64 f = FirstField(strct);
        for (i32 i = 0; i < 4000 && IsPtr(f); i++) { if (FieldName(f, nm, 128) && seq(nm, name)) return f; f = NextField(f); }
        if (!supers) break;
        strct = Super(strct);
    }
    return 0;
}
static i32 PropOff(u64 strct, const char* name) { u64 f = FindField(strct, name, true); return f ? FieldOff(f) : -1; }

static bool ChainHas(u64 cls, const char* name) {
    char nm[128];
    for (i32 d = 0; d < 40 && IsPtr(cls); d++) { if (ObjName(cls, nm, 128) && seq(nm, name)) return true; cls = Super(cls); }
    return false;
}

struct ObjRec { u64 ptr, cls, outer; u32 name, num, gidx; };
static ObjRec* g_objs = 0; static u32 g_objCap = 0, g_objCount = 0;
static u8* g_itemBuf = 0;

static bool ScanObjects() {
    u64 table = Rd<u64>(g_base + RVA_OBJECTS);
    i32 num = Rd<i32>(g_base + RVA_NUMOBJ);
    if (!IsPtr(table) || num <= 0 || num > 8000000) return false;
    if ((u32)num + 4096 > g_objCap) {
        g_objCap = (u32)num + 65536;
        g_objs = (ObjRec*)VirtualAlloc(0, (u64)g_objCap * sizeof(ObjRec), 0x3000, 4);
        if (!g_objs) { g_objCap = 0; return false; }
    }
    if (!g_itemBuf) g_itemBuf = (u8*)VirtualAlloc(0, 65536 * 24, 0x3000, 4);
    u32 cnt = 0;
    i32 chunks = (num + 65535) / 65536;
    for (i32 c = 0; c < chunks; c++) {
        u64 cp = Rd<u64>(table + (u64)c * 8);
        if (!IsPtr(cp)) continue;
        i32 n = num - c * 65536; if (n > 65536) n = 65536;
        if (!RPM(cp, g_itemBuf, (u64)n * 24)) continue;
        for (i32 i = 0; i < n; i++) {
            u64 o = *(u64*)(g_itemBuf + (u64)i * 24);
            if (!IsPtr(o)) continue;
            u8 h[0x28]; if (!RPM(o, h, 0x28)) continue;
            u64 cls = *(u64*)(h + 0x10); if (!IsPtr(cls)) continue;
            if (cnt >= g_objCap) break;
            g_objs[cnt].ptr = o; g_objs[cnt].cls = cls; g_objs[cnt].outer = *(u64*)(h + 0x20); g_objs[cnt].name = *(u32*)(h + 0x18); g_objs[cnt].num = *(u32*)(h + 0x1C); g_objs[cnt].gidx = (u32)(c * 65536 + i);
            cnt++;
        }
    }
    g_objCount = cnt;
    return cnt > 1000;
}
static bool ObjLive(u32 gidx, u64 ptr, u64 cls) {
    u64 table = Rd<u64>(g_base + RVA_OBJECTS); if (!IsPtr(table)) return false;
    i32 num = Rd<i32>(g_base + RVA_NUMOBJ); if ((i32)gidx >= num) return false;
    u64 cp = Rd<u64>(table + (u64)(gidx >> 16) * 8); if (!IsPtr(cp)) return false;
    u8 it[16]; if (!RPM(cp + (u64)(gidx & 0xFFFF) * 24, it, 16)) return false;
    if (*(u64*)it != ptr) return false;
    u32 fl = *(u32*)(it + 8); if (fl & (1u << 21)) return false;
    return Rd<u64>(ptr + 0x10) == cls;
}

struct NameQ { const char* name; const char* metaSuffix; u64 result; };
static void FindObjects(NameQ* q, i32 nq) {
    char nm[128], cn[128];
    for (i32 k = 0; k < nq; k++) q[k].result = 0;
    u32 lastIdx = 0xFFFFFFFF; bool lastHit = false;
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        if (r.num != 0) continue;
        if (r.name == lastIdx && !lastHit) continue;
        if (!NameIdx(r.name, 0, nm, 128)) continue;
        lastIdx = r.name; lastHit = false;
        for (i32 k = 0; k < nq; k++) {
            if (q[k].result || !seq(nm, q[k].name)) continue;
            lastHit = true;
            if (ObjName(r.cls, cn, 128) && sends(cn, q[k].metaSuffix)) q[k].result = r.ptr;
        }
    }
}

static u64 cPC, cController, cActor, cScene, cWorld, cGI, cPlayer, cWidget, cUserWidget, cTextBlock, cTextLayout, cImage, sSlateBrush, sSlateColor, sVector;
static i32 oPCM = -1, oViewTarget = -1, oVTTarget = -1;
static i32 oOGI = -1, oLP = -1, oPCref = -1, oAckPawn = -1, oCtrlPawn = -1, oRoot = -1, oRelLoc = -1, oRelRot = -1, oVel = -1;
static i32 oBrushDrawAs = -1, oTbColor = -1, oTbShadowColor = -1, oTbShadowOff = -1, oTbFont = -1, oFontOutline = -1, oOutlineColor = -1, oOutlineSize = -1, oWidgetTree = -1;
static volatile i32 g_reflReady = 0;
static volatile double g_landZ = 0; static volatile i32 g_landOk = 0;
static u64 cPanelWidget, cPanelSlot, cWidgetTree, cCamera;
static i32 oPanelSlots = -1, oSlotContent = -1, oTreeRoot = -1, oWVis = -1, oFov = -1, oAttach = -1;

static bool DetectLayout() {
    char nm[128];
    u32 supC[] = { 0x40, 0x30, 0x48, 0x38 };
    bool supOk = false;
    for (u32 sc : supC) {
        u64 p = cPC;
        for (i32 d = 0; d < 12 && IsPtr(p); d++) { p = Rd<u64>(p + sc); if (ObjName(p, nm, 128) && seq(nm, "Actor")) { L.sup = sc; supOk = true; break; } }
        if (supOk) break;
    }
    if (!supOk) { Log("layout: SuperStruct offset not found"); return false; }
    u32 chC[] = { 0x50, 0x48, 0x58, 0x40 };
    u32 nnC[][2] = { { 0x20, 0x28 }, { 0x18, 0x20 }, { 0x28, 0x30 } };
    bool fOk = false; u64 fAck = 0;
    for (u32 cc : chC) {
        for (auto& nn : nnC) {
            u64 f = Rd<u64>(cPC + cc);
            for (i32 i = 0; i < 400 && IsPtr(f); i++) {
                u32 ni[2]; if (!RPM(f + nn[1], ni, 8)) break;
                if (NameIdx(ni[0], ni[1], nm, 128) && seq(nm, "AcknowledgedPawn")) { fAck = f; break; }
                f = Rd<u64>(f + nn[0]);
            }
            if (fAck) { L.child = cc; L.fNext = nn[0]; L.fName = nn[1]; fOk = true; break; }
        }
        if (fOk) break;
    }
    if (!fOk) { Log("layout: ChildProperties/FField layout not found"); return false; }
    L.psize = L.child + 8;
    i32 pcSize = PropsSize(cPC);
    u32 E = L.fName + 0x10;
    u32 offC[] = { E + 0x14, E + 0x0C, E + 0x10, E + 0x18, E + 0x1C };
    L.pElem = E + 4;
    if (Rd<i32>(fAck + L.pElem) != 8) { bool got = false; for (u32 x = E - 8; x < E + 0x20; x += 4) if (Rd<i32>(fAck + x) == 8 && Rd<i32>(fAck + x - 4) == 1) { L.pElem = x; got = true; break; } if (!got) { Log("layout: ElementSize not found"); return false; } }
    u64 fRoot = FindField(cActor, "RootComponent", false);
    bool oOk = false;
    for (u32 oc : offC) {
        i32 a = Rd<i32>(fAck + oc), b = fRoot ? Rd<i32>(fRoot + oc) : 0x100;
        if (a > 0x28 && a < pcSize && b > 0x28 && b < 0x1000 && (a & 7) == 0 && (b & 7) == 0) { L.pOff = oc; oOk = true; break; }
    }
    if (!oOk) { Log("layout: Offset_Internal not found"); return false; }
    u64 fLoc = FindField(cScene, "RelativeLocation", false);
    bool xOk = false;
    for (u32 x = E + 0x28; x <= E + 0x58 && fLoc; x += 8) { u64 s = Rd<u64>(fLoc + x); if (ObjName(s, nm, 128) && seq(nm, "Vector")) { L.pExtra = x; xOk = true; break; } }
    if (!xOk) { Log("layout: FStructProperty::Struct not found"); return false; }
    L.arrInner = L.pExtra;
    u64 fLP = FindField(cGI, "LocalPlayers", false);
    if (fLP) {
        for (u32 x = L.pExtra; x <= L.pExtra + 8; x += 8) { u64 in = Rd<u64>(fLP + x); if (IsPtr(in) && FieldType(in, nm, 128) && sends(nm, "Property")) { L.arrInner = x; break; } }
    }
    L.ok = true;
    static char B_[2048]; SB sb{ B_, 0, 2048 };
    sb.s("layout ok: super="); sb.h(L.sup); sb.s(" child="); sb.h(L.child); sb.s(" next="); sb.h(L.fNext); sb.s(" name="); sb.h(L.fName);
    sb.s(" elem="); sb.h(L.pElem); sb.s(" off="); sb.h(L.pOff); sb.s(" extra="); sb.h(L.pExtra); sb.s(" inner="); sb.h(L.arrInner);
    sb.z(); Log(B_);
    return true;
}

static bool InitReflection() {
    char nm[64];
    if (!NameIdx(0, 0, nm, 64) || !seq(nm, "None")) { Log("FNamePool: entry 0 is not 'None' (got '", nm, "')"); return false; }
    if (!ScanObjects()) { Log("GUObjectArray: scan failed or too few objects"); return false; }
    NameQ q[] = {
        { "PlayerController", "Class", 0 }, { "Controller", "Class", 0 }, { "Actor", "Class", 0 }, { "SceneComponent", "Class", 0 },
        { "World", "Class", 0 }, { "GameInstance", "Class", 0 }, { "Player", "Class", 0 }, { "Widget", "Class", 0 },
        { "UserWidget", "Class", 0 }, { "TextBlock", "Class", 0 }, { "TextLayoutWidget", "Class", 0 }, { "Image", "Class", 0 },
        { "SlateBrush", "ScriptStruct", 0 }, { "SlateColor", "ScriptStruct", 0 }, { "Vector", "ScriptStruct", 0 },
        { "SlateFontInfo", "ScriptStruct", 0 }, { "FontOutlineSettings", "ScriptStruct", 0 },
        { "PanelWidget", "Class", 0 }, { "PanelSlot", "Class", 0 }, { "WidgetTree", "Class", 0 }, { "CameraComponent", "Class", 0 },
    };
    FindObjects(q, sizeof(q) / sizeof(q[0]));
    cPC = q[0].result; cController = q[1].result; cActor = q[2].result; cScene = q[3].result; cWorld = q[4].result; cGI = q[5].result; cPlayer = q[6].result;
    cWidget = q[7].result; cUserWidget = q[8].result; cTextBlock = q[9].result; cTextLayout = q[10].result; cImage = q[11].result;
    sSlateBrush = q[12].result; sSlateColor = q[13].result; sVector = q[14].result;
    u64 sFont = q[15].result, sOutline = q[16].result;
    cPanelWidget = q[17].result; cPanelSlot = q[18].result; cWidgetTree = q[19].result; cCamera = q[20].result;
    {
        static char B_[2048]; SB sb{ B_, 0, 2048 };
        sb.s("objects="); sb.i(g_objCount); sb.s(" PC="); sb.h(cPC); sb.s(" Actor="); sb.h(cActor); sb.s(" Scene="); sb.h(cScene); sb.s(" World="); sb.h(cWorld); sb.s(" GI="); sb.h(cGI); sb.s(" Player="); sb.h(cPlayer);
        sb.z(); Log(B_);
    }
    if (!cPC || !cActor || !cScene || !cWorld || !cGI || !cPlayer) { Log("core classes missing, retry later"); return false; }
    if (!DetectLayout()) return false;
    oOGI = PropOff(cWorld, "OwningGameInstance");
    oLP = PropOff(cGI, "LocalPlayers");
    oPCref = PropOff(cPlayer, "PlayerController");
    oAckPawn = PropOff(cPC, "AcknowledgedPawn");
    oCtrlPawn = cController ? PropOff(cController, "Pawn") : -1;
    oPCM = PropOff(cPC, "PlayerCameraManager");
    {
        NameQ q2[] = { { "PlayerCameraManager", "Class", 0 } };
        FindObjects(q2, 1);
        if (q2[0].result) {
            u64 fvt = FindField(q2[0].result, "ViewTarget", true);
            if (fvt) { oViewTarget = FieldOff(fvt); oVTTarget = PropOff(FieldExtra(fvt), "Target"); }
        }
    }
    oRoot = PropOff(cActor, "RootComponent");
    oRelLoc = PropOff(cScene, "RelativeLocation");
    oRelRot = PropOff(cScene, "RelativeRotation");
    oVel = PropOff(cScene, "ComponentVelocity");
    if (sSlateBrush) oBrushDrawAs = PropOff(sSlateBrush, "DrawAs");
    if (cTextBlock) { oTbColor = PropOff(cTextBlock, "ColorAndOpacity"); oTbShadowColor = PropOff(cTextBlock, "ShadowColorAndOpacity"); oTbShadowOff = PropOff(cTextBlock, "ShadowOffset"); oTbFont = PropOff(cTextBlock, "Font"); }
    if (sFont) oFontOutline = PropOff(sFont, "OutlineSettings");
    if (sOutline) { oOutlineColor = PropOff(sOutline, "OutlineColor"); oOutlineSize = PropOff(sOutline, "OutlineSize"); }
    if (cUserWidget) oWidgetTree = PropOff(cUserWidget, "WidgetTree");
    if (cPanelWidget) oPanelSlots = PropOff(cPanelWidget, "Slots");
    if (cPanelSlot) oSlotContent = PropOff(cPanelSlot, "Content");
    if (cWidgetTree) oTreeRoot = PropOff(cWidgetTree, "RootWidget");
    if (cWidget) oWVis = PropOff(cWidget, "Visibility");
    if (cCamera) oFov = PropOff(cCamera, "FieldOfView");
    oAttach = PropOff(cScene, "AttachParent");
    static char B_[2048]; SB sb{ B_, 0, 2048 };
    sb.s("offsets: OGI="); sb.h(oOGI); sb.s(" LocalPlayers="); sb.h(oLP); sb.s(" Player.PC="); sb.h(oPCref); sb.s(" AckPawn="); sb.h(oAckPawn); sb.s(" Ctrl.Pawn="); sb.h(oCtrlPawn);
    sb.s(" PCM="); sb.h(oPCM); sb.s(" ViewTarget="); sb.h(oViewTarget); sb.s("+"); sb.h(oVTTarget);
    sb.s(" Root="); sb.h(oRoot); sb.s(" RelLoc="); sb.h(oRelLoc); sb.s(" RelRot="); sb.h(oRelRot); sb.s(" DrawAs="); sb.h(oBrushDrawAs); sb.s(" Slots="); sb.h(oPanelSlots); sb.s(" Content="); sb.h(oSlotContent); sb.s(" TreeRoot="); sb.h(oTreeRoot); sb.s(" WVis="); sb.h(oWVis); sb.s(" Fov="); sb.h(oFov); sb.s(" Attach="); sb.h(oAttach);
    sb.z(); Log(B_);
    if (oOGI < 0 || oLP < 0 || oPCref < 0 || oAckPawn < 0 || oRoot < 0 || oRelLoc < 0 || oRelRot < 0) { Log("required properties missing"); return false; }
    return true;
}

struct PawnInfo {
    u64 cls; bool kamikaze, recon; char chain[512];
    u64 thrField; i32 thrOff; bool thrDouble; char thrName[64];
    u64 batField; i32 batOff; bool batDouble; char batName[64];
    i32 uiOff, camOff, dgOff, ppOff, brokenOff; u8 brokenMask;
};
static PawnInfo g_pi;

static bool NumericType(const char* t, bool* dbl) { if (seq(t, "FloatProperty")) { *dbl = false; return true; } if (seq(t, "DoubleProperty")) { *dbl = true; return true; } return false; }

static bool FindNumericProp(u64 cls, const char* explicitName, const char* tokens, char* outName, i32* outOff, bool* outDbl) {
    char nm[128], ty[64];
    if (explicitName[0]) {
        u64 f = FindField(cls, explicitName, true);
        if (f && FieldType(f, ty, 64) && NumericType(ty, outDbl)) { scpy(outName, explicitName, 64); *outOff = FieldOff(f); return true; }
        return false;
    }
    char tok[64]; i32 k = 0;
    for (const char* p = tokens;; p++) {
        if (*p == ',' || !*p) {
            tok[k] = 0;
            if (k) {
                u64 s = cls;
                for (i32 d = 0; d < 32 && IsPtr(s); d++) {
                    u64 f = FirstField(s);
                    for (i32 i = 0; i < 4000 && IsPtr(f); i++) {
                        if (FieldName(f, nm, 128) && icontains(nm, tok) && FieldType(f, ty, 64) && NumericType(ty, outDbl)) { scpy(outName, nm, 64); *outOff = FieldOff(f); return true; }
                        f = NextField(f);
                    }
                    s = Super(s);
                }
            }
            k = 0; if (!*p) break;
        } else if (k < 63) tok[k++] = *p;
    }
    return false;
}

static void AnalyzePawnClass(u64 cls) {
    PawnInfo& p = g_pi;
    memset(&p, 0, sizeof(p));
    p.cls = cls;
    char nm[128]; char most[128] = { 0 };
    SB sb{ p.chain, 0, sizeof(p.chain) };
    u64 s = cls;
    for (i32 d = 0; d < 32 && IsPtr(s); d++) {
        if (!ObjName(s, nm, 128)) break;
        if (d == 0) scpy(most, nm, 128);
        if (d) sb.s(" > ");
        sb.s(nm);
        if (ListMatch(C.kamikazeMatch, nm, true)) p.kamikaze = true;
        if (seq(nm, "Pawn")) break;
        s = Super(s);
    }
    sb.z();
    if (ListMatch(C.reconMatch, most, true)) { p.recon = true; p.kamikaze = false; }
    p.thrOff = -1; p.batOff = -1;
    if (FindNumericProp(cls, C.throttleProp, "Throttle,Thrust,Gas", p.thrName, &p.thrOff, &p.thrDouble)) {}
    if (FindNumericProp(cls, C.batteryProp, "Battery,Charge,Energy", p.batName, &p.batOff, &p.batDouble)) {}
    p.uiOff = PropOff(cls, "UI"); p.camOff = PropOff(cls, "Camera"); p.dgOff = PropOff(cls, "DistanceGround"); p.ppOff = PropOff(cls, "PostProcess");
    { u64 fb = FindField(cls, "bIsBroken", true); p.brokenOff = -1; p.brokenMask = 0; if (fb) { p.brokenOff = FieldOff(fb) + Rd<u8>(fb + L.pExtra + 1); p.brokenMask = Rd<u8>(fb + L.pExtra + 2); } }
    static char B_[2048]; SB lb{ B_, 0, 2048 };
    lb.s("pawn class: "); lb.s(p.chain); lb.s(p.kamikaze ? "  => KAMIKAZE (OSD on)" : "  => not kamikaze (OSD off)");
    lb.s(" | ui="); lb.h(p.uiOff); lb.s(" cam="); lb.h(p.camOff); lb.s(" dground="); lb.h(p.dgOff);
    lb.s(" | throttle="); lb.s(p.thrOff >= 0 ? p.thrName : "-"); lb.s(" battery="); lb.s(p.batOff >= 0 ? p.batName : "-");
    lb.z(); Log(B_);
}

static volatile u64 g_pawn = 0;
static volatile i32 g_pawnSerial = 0;
static volatile i32 g_kamikazeActive = 0;
static volatile i32 g_dumpReq = 0;
static volatile i32 g_userHidden = 0;
static i32 g_link = 0;
static volatile u64 g_ackPawn = 0, g_viewTarget = 0;

static u64 ReadPawn() {
    g_link = 0;
    u64 world = Rd<u64>(g_base + RVA_GWORLD); if (!IsPtr(world)) return 0;
    g_link = 1;
    u64 gi = Rd<u64>(world + oOGI); if (!IsPtr(gi)) return 0;
    u64 arr = Rd<u64>(gi + oLP); i32 n = Rd<i32>(gi + oLP + 8); if (!IsPtr(arr) || n <= 0 || n > 8) return 0;
    u64 lp = Rd<u64>(arr); if (!IsPtr(lp)) return 0;
    u64 pc = Rd<u64>(lp + oPCref); if (!IsPtr(pc)) return 0;
    g_link = 2;
    u64 pawn = Rd<u64>(pc + oAckPawn);
    if (!IsPtr(pawn) && oCtrlPawn >= 0) pawn = Rd<u64>(pc + oCtrlPawn);
    if (!IsPtr(pawn) || !IsPtr(ObjClass(pawn))) pawn = 0;
    u64 vt = 0;
    if (oPCM >= 0 && oViewTarget >= 0 && oVTTarget >= 0) {
        u64 pcm = Rd<u64>(pc + oPCM);
        if (IsPtr(pcm)) { vt = Rd<u64>(pcm + oViewTarget + oVTTarget); if (!IsPtr(vt) || !IsPtr(ObjClass(vt))) vt = 0; }
    }
    g_ackPawn = pawn; g_viewTarget = vt;
    if (vt && vt != pc) pawn = vt;
    if (!pawn) return 0;
    g_link = 3;
    return pawn;
}

static u64 PeekPawn() {
    u64 world = Rd<u64>(g_base + RVA_GWORLD); if (!IsPtr(world)) return 0;
    u64 gi = Rd<u64>(world + oOGI); if (!IsPtr(gi)) return 0;
    u64 arr = Rd<u64>(gi + oLP); if (!IsPtr(arr)) return 0;
    u64 lp = Rd<u64>(arr); if (!IsPtr(lp)) return 0;
    u64 pc = Rd<u64>(lp + oPCref); if (!IsPtr(pc)) return 0;
    u64 pawn = Rd<u64>(pc + oAckPawn);
    if (!IsPtr(pawn) && oCtrlPawn >= 0) pawn = Rd<u64>(pc + oCtrlPawn);
    if (!IsPtr(pawn)) pawn = 0;
    if (oPCM >= 0 && oViewTarget >= 0 && oVTTarget >= 0) { u64 pcm = Rd<u64>(pc + oPCM); if (IsPtr(pcm)) { u64 vt = Rd<u64>(pcm + oViewTarget + oVTTarget); if (IsPtr(vt) && vt != pc) pawn = vt; } }
    return pawn;
}
static HANDLE g_wakeEvt = 0; static volatile u64 g_watchPawn = 0; static volatile i32 g_watchOn = 0;
static u32 __stdcall PawnWatch(void*) {
    for (;;) {
        if (!g_watchOn || !g_watchPawn || !g_proc) { Sleep(5); continue; }
        u64 p = PeekPawn();
        if (p != g_watchPawn) { g_watchOn = 0; if (g_wakeEvt) SetEvent(g_wakeEvt); }
        Sleep(1);
    }
    return 0;
}

static i32 g_slateVis = -1; static bool g_slateVisTried = false;

static u64 g_clsCacheK[512]; static u8 g_clsCacheV[512];
static u8 WidgetKind(u64 cls) {
    for (i32 i = 0; i < 512; i++) { if (g_clsCacheK[i] == cls) return g_clsCacheV[i]; if (!g_clsCacheK[i]) break; }
    u8 k = 0;
    if (ChainHas(cls, "Widget")) k |= 1;
    if (cTextBlock && ChainHas(cls, "TextBlock")) k |= 2;
    if (ChainHas(cls, "UserWidget")) k |= 4;
    if (ChainHas(cls, "PanelWidget")) k |= 8;
    if (ChainHas(cls, "Image")) k |= 16;
    for (i32 i = 0; i < 512; i++) if (!g_clsCacheK[i]) { g_clsCacheK[i] = cls; g_clsCacheV[i] = k; break; }
    return k;
}

static void CollectBrushes(u64 addr, u64 strct, i32 depth, u64 stopAt, u64* out, i32* n, i32 max) {
    char ty[64], sn[128];
    for (i32 d = 0; d < 32 && IsPtr(strct) && strct != stopAt; d++) {
        u64 f = FirstField(strct);
        for (i32 i = 0; i < 2000 && IsPtr(f); i++) {
            if (FieldType(f, ty, 64) && seq(ty, "StructProperty")) {
                u64 st = FieldExtra(f);
                if (ObjName(st, sn, 128)) {
                    i32 off = FieldOff(f);
                    if (seq(sn, "SlateBrush")) { if (oBrushDrawAs >= 0 && *n < max) out[(*n)++] = addr + off + oBrushDrawAs; }
                    else if (depth < 3 && !seq(sn, "Vector") && !seq(sn, "Vector2D") && !seq(sn, "LinearColor") && !seq(sn, "Margin") && !seq(sn, "SlateColor") && !seq(sn, "SlateFontInfo") && !seq(sn, "WidgetTransform"))
                        CollectBrushes(addr + off, st, depth + 1, 0, out, n, max);
                }
            }
            f = NextField(f);
        }
        strct = Super(strct);
    }
}

static bool FindSharedPtr(u64 obj, i32 from, i32 to, u64* outP) {
    if (from < 0x28 || to <= from || to - from > 0x800) return false;
    u8 buf[0x800]; if (!RPM(obj + from, buf, to - from)) return false;
    for (i32 o = 0; o + 16 <= to - from; o += 8) {
        u64 p = *(u64*)(buf + o), rc = *(u64*)(buf + o + 8);
        if (!IsPtr(p) || !IsPtr(rc) || InModule(p) || InModule(rc)) continue;
        if (!InModule(Rd<u64>(p)) || !InModule(Rd<u64>(rc))) continue;
        i32 sr = Rd<i32>(rc + 8), wr = Rd<i32>(rc + 12);
        if (sr < 1 || sr > 4096 || wr < 1 || wr > 4096) continue;
        *outP = p; return true;
    }
    return false;
}
static u64 SlateOf(u64 w) { u64 p = 0; return (cWidget && FindSharedPtr(w, 0x28, PropsSize(cWidget), &p)) ? p : 0; }

struct Hash { u64* k; u64* v; u32 cap; };
static Hash g_outerMap = { 0, 0, 0 };
static void HashBuild() {
    u32 need = 1; while (need < g_objCount * 2 + 16) need <<= 1;
    if (need > g_outerMap.cap) { g_outerMap.cap = need; g_outerMap.k = (u64*)VirtualAlloc(0, (u64)need * 8, 0x3000, 4); g_outerMap.v = (u64*)VirtualAlloc(0, (u64)need * 8, 0x3000, 4); }
    else { memset(g_outerMap.k, 0, (u64)g_outerMap.cap * 8); }
    for (u32 i = 0; i < g_objCount; i++) {
        u64 key = g_objs[i].ptr; u32 h = (u32)((key >> 4) * 2654435761u) & (g_outerMap.cap - 1);
        while (g_outerMap.k[h] && g_outerMap.k[h] != key) h = (h + 1) & (g_outerMap.cap - 1);
        g_outerMap.k[h] = key; g_outerMap.v[h] = i;
    }
}
static i64 HashFind(u64 key) {
    if (!g_outerMap.cap) return -1;
    u32 h = (u32)((key >> 4) * 2654435761u) & (g_outerMap.cap - 1);
    while (g_outerMap.k[h]) { if (g_outerMap.k[h] == key) return (i64)g_outerMap.v[h]; h = (h + 1) & (g_outerMap.cap - 1); }
    return -1;
}
static bool IsUnder(u64 obj, u64 root) {
    for (i32 d = 0; d < 10; d++) {
        i64 i = HashFind(obj); if (i < 0) return false;
        u64 o = g_objs[i].outer; if (o == root) return true; if (!o) return false; obj = o;
    }
    return false;
}
static u64 g_hudInst[16]; static i32 g_hudN = 0;
static void FindHudInstances() {
    g_hudN = 0;
    char cn[128], nm[128];
    for (u32 i = 0; i < g_objCount && g_hudN < 16; i++) {
        ObjRec& r = g_objs[i];
        if (!NameIdx(Rd<u32>(r.cls + 0x18), 0, cn, 128)) continue;
        if (!ListMatch(C.hudWidgets, cn, false)) continue;
        if (!NameIdx(r.name, r.num, nm, 128) || sstarts(nm, "Default__")) continue;
        g_hudInst[g_hudN++] = r.ptr;
    }
}

static volatile i32 g_uiOff = -1;
static volatile i32 g_camOff = -1;
static i32 oCamConstrain = -1; static u8 g_camMask = 0; static i32 oCamAspect = -1; static u64 g_camFixed = 0;
static i32 oPPBlend = -1; static bool g_ppInit = false;
static volatile i32 g_ppOff = -1; static volatile float g_ppWv = -1; static volatile i32 g_winH = 0;
static void KillInterference(u64 pawn) {
    if (!g_ppInit) {
        g_ppInit = true;
        NameQ q[] = { { "PostProcessComponent", "Class", 0 } };
        FindObjects(q, 1);
        if (q[0].result) oPPBlend = PropOff(q[0].result, "BlendWeight");
        static char B_[128]; SB sb{ B_, 0, 128 }; sb.s("interference: PostProcessComponent.BlendWeight @"); sb.h(oPPBlend); sb.z(); Log(B_);
    }
    if (oPPBlend < 0 || g_ppOff < 0 || !IsPtr(pawn)) return;
    u64 pp = Rd<u64>(pawn + g_ppOff); if (!IsPtr(pp)) return;
    float w = Rd<float>(pp + oPPBlend); g_ppWv = w;
    if (w != 0.0f) { float z = 0.0f; WPM(pp + oPPBlend, &z, 4); }
}

static u64 g_cvSP = 0; static i32 g_cvSPoff = -1; static float g_cvSPorig = 0; static bool g_cvSPsaved = false, g_cvTried = false;
static u8* g_memChunk = 0;
static bool ScanMem(bool (*fn)(u64 base, const u8* buf, u64 n, void* ctx), void* ctx) {
    if (!g_memChunk) g_memChunk = (u8*)VirtualAlloc(0, 1 << 20, 0x3000, 4);
    if (!g_memChunk) return false;
    u64 a = 0x10000; MBI m;
    while (a < 0x7FFFFFFF0000ull && VirtualQueryEx(g_proc, (void*)a, &m, sizeof(m)) == sizeof(m)) {
        u64 next = m.base + m.size; if (next <= a) break;
        if (m.state == 0x1000 && m.type == 0x20000 && (m.prot == 0x04 || m.prot == 0x40) ) {
            for (u64 off = 0; off < m.size; off += (1 << 20) - 64) {
                u64 n = m.size - off; if (n > (1 << 20)) n = 1 << 20;
                u64 rd = 0;
                if (!ReadProcessMemory(g_proc, (void*)(m.base + off), g_memChunk, n, &rd) || rd < 64) continue;
                if (fn(m.base + off, g_memChunk, rd, ctx)) return true;
                if (n < (1 << 20)) break;
            }
        }
        a = next;
    }
    return false;
}
struct CVar { wc name[64]; i32 len; double value; bool isInt; u64 obj; u64 str[8]; i32 nStr; u64 data; i32 kind; bool logged; double orig; };
static CVar g_cv[48]; static i32 g_cvN = 0; static volatile i32 g_cvReady = 0;
static void AddCVar(const wc* name, double v, bool isInt) {
    if (g_cvN >= 48) return;
    CVar& c = g_cv[g_cvN++]; memset(&c, 0, sizeof(c)); wcpy(c.name, name, 64); c.len = wlen(c.name); c.value = v; c.isInt = isInt;
}
static bool FindStrFn(u64 base, const u8* buf, u64 n, void* ctx) {
    for (u64 i = 0; i + 130 <= n; i += 16) {
        if (buf[i + 1] != 0 || buf[i + 3] != 0) continue;
        u8 ch = buf[i]; if (ch != 'r' && ch != 'f' && ch != 'g' && ch != 's') continue;
        for (i32 k = 0; k < g_cvN; k++) {
            CVar& c = g_cv[k];
            if ((u8)c.name[0] != ch || c.nStr >= 8) continue;
            bool m = true;
            for (i32 q = 0; q < c.len; q++) if (buf[i + q * 2] != (u8)c.name[q] || buf[i + q * 2 + 1] != 0) { m = false; break; }
            if (!m || buf[i + c.len * 2] != 0 || buf[i + c.len * 2 + 1] != 0) continue;
            c.str[c.nStr++] = base + i;
        }
    }
    return false;
}
static bool FindRefFn(u64 base, const u8* buf, u64 n, void* ctx) {
    for (u64 i = 0; i + 24 <= n; i += 8) {
        u64 v = *(const u64*)(buf + i);
        if (!IsPtr(v)) continue;
        for (i32 k = 0; k < g_cvN; k++) {
            CVar& c = g_cv[k]; if (c.obj) continue;
            for (i32 q = 0; q < c.nStr; q++) {
                if (c.str[q] != v || *(const i32*)(buf + i + 8) != c.len + 1) continue;
                u64 obj = *(const u64*)(buf + i + 16);
                if (IsPtr(obj) && InModule(Rd<u64>(obj))) c.obj = obj;
            }
        }
    }
    return false;
}
static bool PlausibleVal(u32 bits, bool isInt) {
    if (isInt) { i32 v = (i32)bits; return v > -1000000 && v < 1000000; }
    float f; memcpy(&f, &bits, 4); if (f != f) return false;
    if (f == 0.0f) return true;
    float a = f < 0 ? -f : f; return a > 1e-20f && a < 1e7f;
}
static void ResolveCVars() {
    u64 t0 = GetTickCount64();
    ScanMem(FindStrFn, 0);
    ScanMem(FindRefFn, 0);
    for (i32 k = 0; k < g_cvN; k++) {
        CVar& c = g_cv[k];
        if (!c.obj) continue;
        u64 p = Rd<u64>(c.obj + 0x50);
        if (InModule(p)) { c.kind = 2; c.data = p; if (!PlausibleVal(Rd<u32>(p), c.isInt)) c.kind = 0; }
        else {
            u32 a = Rd<u32>(c.obj + 0x50), b = Rd<u32>(c.obj + 0x54);
            if (a == b && PlausibleVal(a, c.isInt)) { c.kind = 1; c.data = c.obj + 0x50; }
        }
        if (c.kind) { u32 bits = Rd<u32>(c.data); if (c.isInt) c.orig = (i32)bits; else { float f; memcpy(&f, &bits, 4); c.orig = f; } }
    }
    for (i32 k = 0; k < g_cvN; k++) {
        CVar& c = g_cv[k];
        static char B_[256]; SB sb{ B_, 0, 256 };
        sb.s("cvar "); for (i32 q = 0; q < c.len; q++) sb.c((char)c.name[q]);
        sb.s(": strings="); sb.i(c.nStr); sb.s(" obj="); sb.h(c.obj);
        sb.s(c.kind == 1 ? " inline" : c.kind == 2 ? " ref" : " NOT USABLE");
        if (c.kind) { sb.s(" was="); sb.f(c.orig, 3); if (k) { sb.s(" set="); sb.f(c.value, 3); } }
        sb.z(); Log(B_);
    }
    if (g_cvN && g_cv[0].kind == 1) { g_cvSP = g_cv[0].obj; g_cvSPoff = 0x50; }
    static char B_[64]; SB sb{ B_, 0, 64 }; sb.s("cvar scan ms="); sb.i((i64)(GetTickCount64() - t0)); sb.z(); Log(B_);
    g_cvReady = 1;
}
static u64 g_scal = 0; static bool g_scalTried = false; static double g_vds = 0, g_smlod = 0; static bool g_smlodOk = false;
static void FindScalabilityCache() {
    g_scalTried = true;
    double smOrig = -1;
    for (i32 k = 1; k < g_cvN; k++) {
        const wc* n = g_cv[k].name;
        const wc* t = L"r.ViewDistanceScale"; bool eq = true; for (i32 q = 0; n[q] || t[q]; q++) if (n[q] != t[q]) { eq = false; break; } if (eq) g_vds = g_cv[k].value;
        const wc* t2 = L"r.StaticMeshLODDistanceScale"; eq = true; for (i32 q = 0; n[q] || t2[q]; q++) if (n[q] != t2[q]) { eq = false; break; } if (eq) { g_smlod = g_cv[k].value; smOrig = g_cv[k].kind ? g_cv[k].orig : -1; }
    }
    if (g_vds <= 0) return;
    u64 dataRva = 0, dataSize = 0;
    { i32 pe = Rd<i32>(g_base + 0x3C); u16 ns = Rd<u16>(g_base + pe + 6), osz = Rd<u16>(g_base + pe + 20);
      for (i32 i = 0; i < ns; i++) { u64 sh = g_base + pe + 24 + osz + (u64)i * 40; char nm[9]; RPM(sh, nm, 8); nm[8] = 0;
        if (seq(nm, ".data")) { dataSize = Rd<u32>(sh + 8); dataRva = Rd<u32>(sh + 12); break; } } }
    if (!dataRva || !dataSize || dataSize > 0x4000000) return;
    u8* buf = (u8*)VirtualAlloc(0, dataSize + 16, 0x3000, 4); if (!buf) return;
    for (u64 o = 0; o < dataSize; o += 0x1000) RPM(g_base + dataRva + o, buf + o, (dataSize - o) < 0x1000 ? (dataSize - o) : 0x1000);
    i32 cands = 0; u64 found = 0;
    for (u64 o = 8; o + 8 <= dataSize; o += 4) {
        float a = *(float*)(buf + o), b = *(float*)(buf + o + 4);
        if (!(a >= 0.1f && a <= 20.0f) || b != a * a) continue;
        i32 csm = *(i32*)(buf + o - 4), sh = *(i32*)(buf + o - 8);
        if (csm < 256 || csm > 16384 || (csm & (csm - 1)) || sh < 256 || sh > 16384 || (sh & (sh - 1))) continue;
        cands++; found = g_base + dataRva + o;
    }
    static char B_[160]; SB sb{ B_, 0, 160 };
    sb.s("scalability cache: candidates="); sb.i(cands);
    if (cands == 1) {
        g_scal = found; sb.s(" ViewDistanceScale was="); sb.f(Rd<float>(found), 2); sb.s(" set="); sb.f(g_vds, 2);
        i32 aniso = Rd<i32>(found + 8); float sm = Rd<float>(found + 12);
        sb.s(" | aniso="); sb.i(aniso); sb.s(" smlod="); sb.f(sm, 3);
        if (g_smlod > 0 && aniso >= 0 && aniso <= 16 && smOrig > 0 && sm == (float)smOrig) { g_smlodOk = true; sb.s(" -> StaticMeshLODDistanceScale set="); sb.f(g_smlod, 3); }
        else sb.s(" (StaticMeshLOD slot not confirmed, skipped)");
    }
    sb.z(); Log(B_);
    { static char H_[256]; SB hb{ H_, 0, 256 }; hb.s("scalability bytes:"); for (i32 k = -16; k < 32; k += 4) { hb.c(' '); hb.h(Rd<u32>(found + k)); } hb.z(); if (cands == 1) Log(H_); }
}
static volatile i32 g_gfxOff = 0; static double g_vdsOrig = 1.0, g_smlodOrig = 1.0; static bool g_scalOrigSaved = false;
static void ApplyScalabilityCache() {
    if (!g_scal) return;
    if (!g_scalOrigSaved) { g_scalOrigSaved = true; g_vdsOrig = Rd<float>(g_scal); g_smlodOrig = Rd<float>(g_scal + 12); }
    double vds = g_gfxOff ? g_vdsOrig : g_vds, sm = g_gfxOff ? g_smlodOrig : g_smlod;
    float v[2] = { (float)vds, (float)(vds * vds) }, cur[2];
    if (RPM(g_scal, cur, 8) && (cur[0] != v[0] || cur[1] != v[1])) WPM(g_scal, v, 8);
    if (g_smlodOk) { float w = (float)sm; if (Rd<float>(g_scal + 12) != w) WPM(g_scal + 12, &w, 4); }
}
static void ApplyCVars() {
    if (g_cvReady && !g_scalTried) FindScalabilityCache();
    ApplyScalabilityCache();
    if (!g_cvReady) return;
    for (i32 k = 1; k < g_cvN; k++) {
        CVar& c = g_cv[k]; if (!c.kind) continue;
        double tv = g_gfxOff ? c.orig : c.value;
        u32 want; if (c.isInt) want = (u32)(i32)tv; else { float f = (float)tv; memcpy(&want, &f, 4); }
        u32 cur = Rd<u32>(c.data);
        if (cur == want) continue;
        if (c.kind == 1) { u32 v[2] = { want, want }; WPM(c.data, v, 8); }
        else WPM(c.data, &want, 4);
    }
}
static void ApplyScreenPercentage(bool on, i32 winH) {
    if (!g_cvSP || g_cvSPoff < 0) return;
    float cur = Rd<float>(g_cvSP + g_cvSPoff);
    if (on && winH > 0) {
        if (!g_cvSPsaved) { g_cvSPorig = cur; g_cvSPsaved = true; }
        float want = (float)C.camHeight * 100.0f / (float)winH;
        if (want > g_cvSPorig) want = g_cvSPorig; if (want < 10.0f) want = 10.0f;
        if (cur != want) { float v[2] = { want, want }; WPM(g_cvSP + g_cvSPoff, v, 8); }
    } else if (g_cvSPsaved) {
        float v[2] = { g_cvSPorig, g_cvSPorig }; WPM(g_cvSP + g_cvSPoff, v, 8); g_cvSPsaved = false;
    }
}

static u32 __stdcall SPThread(void*) { ResolveCVars(); return 0; }
static const char* DEFAULT_CVARS =
    "\r\n[CVars]\r\n"
    "foliage.LODDistanceScale=3.0\r\n"
    "r.StaticMeshLODDistanceScale=0.5\r\n"
    "r.ViewDistanceScale=3.0\r\n"
    "grass.CullDistanceScale=8.0\r\n"
    "r.Shadow.DistanceScale=4.0\r\n"
    "r.Shadow.RadiusThreshold=0.002\r\n"
    "r.Shadow.MaxCSMResolution=4096\r\n"
    "foliage.CullDistanceScale=1.0\r\n"
    "foliage.DitheredLOD=1\r\n"
    "r.Nanite.MaxPixelsPerEdge=0.3\r\n"
    "r.Shadow.Virtual.ResolutionLodBiasDirectional=-2.5\r\n"
    "r.Shadow.Virtual.ResolutionLodBiasDirectionalMoving=-2.5\r\n";
static wc g_secBuf[8192];
static void LoadCVarConfig() {
    g_cvN = 0;
    AddCVar(L"r.ScreenPercentage", 0, false);
    u32 n = GetPrivateProfileSectionW(L"CVars", g_secBuf, 8192, g_cfgPath);
    for (wc* p = g_secBuf; *p; p += wlen(p) + 1) {
        if (*p == ';') continue;
        wc* eq = p; while (*eq && *eq != '=') eq++;
        if (!*eq) continue;
        *eq = 0; const wc* val = eq + 1;
        bool isInt = true; for (const wc* q = val; *q; q++) if (*q == '.' || *q == ',') isInt = false;
        double v = ParseDbl(val, 0);
        if (wlen(p) > 0 && wlen(p) < 63) AddCVar(p, v, isInt);
        *eq = '=';
    }
}
static i32 oCamFovLod = -1; static u8 g_camFovMask = 0; static u64 g_camFovFixed = 0;
static void FixCamera(u64 pawn) {
    if (!cCamera || g_camOff < 0 || !IsPtr(pawn)) return;
    if (oCamConstrain == -1) {
        u64 f = FindField(cCamera, "bConstrainAspectRatio", true);
        if (!f) { oCamConstrain = -2; Log("camera: bConstrainAspectRatio not found"); }
        else { oCamConstrain = FieldOff(f) + Rd<u8>(f + L.pExtra + 1); g_camMask = Rd<u8>(f + L.pExtra + 2); }
        oCamAspect = PropOff(cCamera, "AspectRatio");
        u64 g = FindField(cCamera, "bUseFieldOfViewForLOD", true);
        if (g) { oCamFovLod = FieldOff(g) + Rd<u8>(g + L.pExtra + 1); g_camFovMask = Rd<u8>(g + L.pExtra + 2); }
        else Log("camera: bUseFieldOfViewForLOD not found");
    }
    u64 cam = Rd<u64>(pawn + g_camOff); if (!IsPtr(cam)) return;
    if (C.fullCam && oCamConstrain >= 0 && g_camMask) {
        u8 b = Rd<u8>(cam + oCamConstrain);
        if (b & g_camMask) {
            if (cam != g_camFixed) {
                g_camFixed = cam;
                static char B_[256]; SB sb{ B_, 0, 256 }; sb.s("camera: aspect was locked to "); sb.f(oCamAspect >= 0 ? Rd<float>(cam + oCamAspect) : 0, 3); sb.s(", unlocking for fullscreen"); sb.z(); Log(B_);
            }
            b &= (u8)~g_camMask; WPM(cam + oCamConstrain, &b, 1);
        }
    }
    if (false && oCamFovLod >= 0 && g_camFovMask) {
        u8 b = Rd<u8>(cam + oCamFovLod);
        if (b & g_camFovMask) {
            if (cam != g_camFovFixed) { g_camFovFixed = cam; Log("camera: LOD/cull no longer scaled by wide FOV"); }
            b &= (u8)~g_camFovMask; WPM(cam + oCamFovLod, &b, 1);
        }
    }
}
static u64 g_uiCls = 0; static i32 g_uiWidgetOff = -1;
static u64 CurrentHud(u64 pawn, i32 uiOff) {
    if (!IsPtr(pawn) || uiOff < 0) return 0;
    u64 ui = Rd<u64>(pawn + uiOff); if (!IsPtr(ui)) return 0;
    u64 uc = ObjClass(ui); if (!IsPtr(uc)) return 0;
    if (uc != g_uiCls) { g_uiCls = uc; g_uiWidgetOff = PropOff(uc, "Widget"); }
    if (g_uiWidgetOff < 0) return 0;
    u64 w = Rd<u64>(ui + g_uiWidgetOff); if (!IsPtr(w)) return 0;
    char cn[128]; if (!ClassNameOf(w, cn, 128) || !ListMatch(C.hudWidgets, cn, false)) return 0;
    return w;
}
static i32 CollectTree(u64 uw, u64* out, i32 max) {
    if (oWidgetTree < 0 || oTreeRoot < 0) return 0;
    u64 tree = Rd<u64>(uw + oWidgetTree); if (!IsPtr(tree)) return 0;
    u64 st[256]; i32 sp = 0, n = 0;
    st[sp++] = Rd<u64>(tree + oTreeRoot);
    while (sp && n < max) {
        u64 w = st[--sp]; if (!IsPtr(w)) continue;
        u64 cls = ObjClass(w); if (!IsPtr(cls)) continue;
        u8 k = WidgetKind(cls); if (!(k & 1)) continue;
        out[n++] = w;
        if ((k & 8) && oPanelSlots >= 0 && oSlotContent >= 0) {
            u64 arr = Rd<u64>(w + oPanelSlots); i32 cnt = Rd<i32>(w + oPanelSlots + 8);
            if (IsPtr(arr) && cnt > 0 && cnt < 256) {
                u64 slots[256]; if (RPM(arr, slots, (u64)cnt * 8))
                    for (i32 i = 0; i < cnt && sp < 256; i++) if (IsPtr(slots[i])) st[sp++] = Rd<u64>(slots[i] + oSlotContent);
            }
        }
    }
    return n;
}
static u8 VisMap(u8 v) { static const u8 m[5] = { 0x19, 0x02, 0x04, 0x01, 0x11 }; return v < 5 ? m[v] : 0xFF; }
static u8 g_visBuf[64][0x300];
static void DetectSlateVis(u64* ws, i32 n) {
    if (oWVis < 0) return;
    u8 exp[64]; i32 m = 0;
    for (i32 i = 0; i < n && m < 64; i++) {
        u8 k = WidgetKind(ObjClass(ws[i])); if (k & 4) continue;
        u64 s = SlateOf(ws[i]); if (!s) continue;
        u8 e = VisMap(Rd<u8>(ws[i] + oWVis)); if (e == 0xFF) continue;
        if (!RPM(s, g_visBuf[m], 0x300)) continue;
        exp[m++] = e;
    }
    if (m < 4) return;
    g_slateVisTried = true;
    static char B_[512]; SB sb{ B_, 0, 512 };
    sb.s("slate visibility detect: samples="); sb.i(m);
    i32 found = -1, cands = 0;
    for (i32 o = 8; o < 0x300; o++) {
        bool ok = true; for (i32 i = 0; i < m && ok; i++) if (g_visBuf[i][o] != exp[i]) ok = false;
        if (ok) { cands++; if (found < 0) found = o; }
    }
    sb.s(" candidates="); sb.i(cands);
    if (cands == 1) { g_slateVis = found; sb.s(" offset="); sb.h(found); }
    sb.z(); Log(B_);
}

struct VisSave { u64 p; u8 orig; };
static VisSave g_vs[256]; static i32 g_vsN = 0; static u64 g_hudCur = 0;
static void HideHud(u64 hud) {
    u64 ws[256]; i32 n = CollectTree(hud, ws, 256);
    if (!n) return;
    if (g_slateVis < 0 && !g_slateVisTried) DetectSlateVis(ws, n);
    if (g_slateVis >= 0) {
        u64 targets[257]; i32 tn = 0; char nm[128];
        if (!C.keepWidgets[0]) { u64 s = SlateOf(hud); if (s) targets[tn++] = s; }
        else for (i32 i = 0; i < n; i++) {
            if (ObjName(ws[i], nm, 128) && ListMatch(C.keepWidgets, nm, false)) continue;
            u8 k = WidgetKind(ObjClass(ws[i])); if (k & 8) continue;
            u64 s = SlateOf(ws[i]); if (s) targets[tn++] = s;
        }
        u8 hid = 0x04;
        for (i32 i = 0; i < tn; i++) {
            u8 cur = Rd<u8>(targets[i] + g_slateVis);
            if (cur == hid) continue;
            if (cur != 0x19 && cur != 0x01 && cur != 0x11 && cur != 0x02) continue;
            bool have = false; for (i32 k = 0; k < g_vsN; k++) if (g_vs[k].p == targets[i]) { have = true; break; }
            if (!have && g_vsN < 256) { g_vs[g_vsN].p = targets[i]; g_vs[g_vsN].orig = cur; g_vsN++; }
            WPM(targets[i] + g_slateVis, &hid, 1);
        }
    } else if (C.hideImages) {
        u64 br[512]; i32 bn = 0; u8 z = 0;
        for (i32 i = 0; i < n; i++) { u8 k = WidgetKind(ObjClass(ws[i])); if (!(k & 4)) CollectBrushes(ws[i], ObjClass(ws[i]), 0, cWidget, br, &bn, 512); }
        for (i32 i = 0; i < bn; i++) { u8 cur = Rd<u8>(br[i]); if (cur != 0 && cur <= 4) WPM(br[i], &z, 1); }
    }
}
static void RestoreHud() {
    for (i32 i = 0; i < g_vsN; i++) { u8 cur = Rd<u8>(g_vs[i].p + g_slateVis); if (cur == 0x04) WPM(g_vs[i].p + g_slateVis, &g_vs[i].orig, 1); }
    g_vsN = 0;
}
static char* g_dump = 0; static const u32 DUMPCAP = 4 << 20;
static void DumpValue(SB& sb, u64 addr, u64 f, const char* ty, i32 depth, i32 indent);
static void Indent(SB& sb, i32 n) { for (i32 i = 0; i < n; i++) sb.s("  "); }
static const char* g_dumpStop = 0;
static void DumpStruct(SB& sb, u64 addr, u64 strct, i32 depth, i32 indent, bool onlyBP) {
    char nm[128], ty[64], cn[128];
    for (i32 d = 0; d < 32 && IsPtr(strct); d++) {
        if (!ObjName(strct, cn, 128)) break;
        if (onlyBP && !sends(cn, "_C")) break;
        if (g_dumpStop && seq(cn, g_dumpStop)) break;
        Indent(sb, indent); sb.s("["); sb.s(cn); sb.s("] size="); sb.h(PropsSize(strct)); sb.s("\r\n");
        u64 f = FirstField(strct);
        for (i32 i = 0; i < 3000 && IsPtr(f); i++) {
            FieldName(f, nm, 128); FieldType(f, ty, 64);
            i32 off = FieldOff(f);
            Indent(sb, indent + 1); sb.s(nm); sb.s(" : "); sb.s(ty); sb.s(" @"); sb.h(off); sb.s(" = ");
            DumpValue(sb, addr + off, f, ty, depth, indent + 1);
            sb.s("\r\n");
            if (sb.n > DUMPCAP - 4096) return;
            f = NextField(f);
        }
        strct = Super(strct);
    }
}
static void DumpObjRef(SB& sb, u64 p) {
    char nm[128], cn[128];
    if (!p) { sb.s("null"); return; }
    if (!IsPtr(p)) { sb.h(p); return; }
    ObjName(p, nm, 128); ClassNameOf(p, cn, 128); sb.s(nm); sb.s(" ("); sb.s(cn); sb.s(")");
}
static void DumpValue(SB& sb, u64 a, u64 f, const char* ty, i32 depth, i32 indent) {
    char nm[128];
    if (seq(ty, "FloatProperty")) sb.f(Rd<float>(a), 4);
    else if (seq(ty, "DoubleProperty")) sb.f(Rd<double>(a), 4);
    else if (seq(ty, "IntProperty")) sb.i(Rd<i32>(a));
    else if (seq(ty, "Int64Property")) sb.i(Rd<i64>(a));
    else if (seq(ty, "UInt32Property")) sb.i(Rd<u32>(a));
    else if (seq(ty, "Int16Property")) sb.i(Rd<i16>(a));
    else if (seq(ty, "UInt16Property")) sb.i(Rd<u16>(a));
    else if (seq(ty, "ByteProperty")) sb.i(Rd<u8>(a));
    else if (seq(ty, "BoolProperty")) { u8 bo = Rd<u8>(f + L.pExtra + 1), bm = Rd<u8>(f + L.pExtra + 2); sb.s((Rd<u8>(a + bo) & bm) ? "true" : "false"); }
    else if (seq(ty, "EnumProperty")) { i32 es = FieldElem(f); if (es == 1) sb.i(Rd<u8>(a)); else if (es == 2) sb.i(Rd<u16>(a)); else sb.i(Rd<i32>(a)); }
    else if (seq(ty, "NameProperty")) { u32 ni[2]; RPM(a, ni, 8); NameIdx(ni[0], ni[1], nm, 128); sb.s(nm); }
    else if (seq(ty, "StrProperty")) { u64 dp = Rd<u64>(a); i32 n = Rd<i32>(a + 8); sb.c('"'); if (IsPtr(dp) && n > 0 && n < 200) { u16 w[200]; RPM(dp, w, n * 2); for (i32 k = 0; k < n - 1; k++) sb.c(w[k] < 128 ? (char)w[k] : '?'); } sb.c('"'); }
    else if (seq(ty, "ObjectProperty") || seq(ty, "ClassProperty") || seq(ty, "InterfaceProperty")) DumpObjRef(sb, Rd<u64>(a));
    else if (seq(ty, "WeakObjectProperty")) { sb.s("weak idx="); sb.i(Rd<i32>(a)); }
    else if (seq(ty, "StructProperty")) {
        u64 st = FieldExtra(f); ObjName(st, nm, 128); sb.s("{"); sb.s(nm); sb.s("}");
        if (seq(nm, "Vector") || seq(nm, "Rotator")) { double v[3]; RPM(a, v, 24); sb.s(" "); sb.f(v[0], 2); sb.s(", "); sb.f(v[1], 2); sb.s(", "); sb.f(v[2], 2); }
        else if (seq(nm, "Vector2D")) { double v[2]; RPM(a, v, 16); sb.s(" "); sb.f(v[0], 2); sb.s(", "); sb.f(v[1], 2); }
        else if (seq(nm, "LinearColor")) { float v[4]; RPM(a, v, 16); sb.s(" "); for (i32 k = 0; k < 4; k++) { sb.f(v[k], 3); if (k < 3) sb.s(", "); } }
        else if (depth > 0) { sb.s("\r\n"); DumpStruct(sb, a, st, depth - 1, indent + 1, false); }
    }
    else if (seq(ty, "ArrayProperty")) {
        u64 dp = Rd<u64>(a); i32 n = Rd<i32>(a + 8); sb.s("num="); sb.i(n);
        u64 in = Rd<u64>(f + L.arrInner); char ity[64]; FieldType(in, ity, 64); sb.s(" of "); sb.s(ity);
        if (n > 0 && n < 100000 && IsPtr(dp) && depth > 0) {
            i32 es = FieldElem(in); i32 show = n < 40 ? n : 40;
            for (i32 k = 0; k < show; k++) {
                sb.s("\r\n"); Indent(sb, indent + 1); sb.s("["); sb.i(k); sb.s("] ");
                DumpValue(sb, dp + (u64)k * es, in, ity, depth - 1, indent + 1);
                if (sb.n > DUMPCAP - 4096) return;
            }
        }
    }
    else sb.s("?");
}

static void DoDump(const char* reason) {
    if (!g_dump) g_dump = (char*)VirtualAlloc(0, DUMPCAP, 0x3000, 4);
    if (!g_dump) return;
    SB sb{ g_dump, 0, DUMPCAP };
    SYSTEMTIME t; GetLocalTime(&t);
    sb.s("\r\n==================== DUMP "); sb.i(t.h); sb.c(':'); sb.i(t.mi); sb.c(':'); sb.i(t.s); sb.s(" ("); sb.s(reason); sb.s(") ====================\r\n");
    ScanObjects(); HashBuild();
    u64 pawn = g_pawn;
    sb.s("link="); sb.i(g_link); sb.s(" used="); DumpObjRef(sb, pawn); sb.s("\r\nAcknowledgedPawn="); DumpObjRef(sb, g_ackPawn); sb.s("\r\nViewTarget="); DumpObjRef(sb, g_viewTarget); sb.s("\r\n");
    if (IsPtr(pawn)) {
        u64 cls = ObjClass(pawn);
        sb.s("chain: "); char nm[128];
        for (u64 s = cls; IsPtr(s); s = Super(s)) { if (!ObjName(s, nm, 128)) break; sb.s(nm); sb.s(" > "); if (seq(nm, "Object")) break; }
        sb.s("\r\n");
        u64 root = Rd<u64>(pawn + oRoot);
        sb.s("root: "); DumpObjRef(sb, root);
        double v[3]; RPM(root + oRelLoc, v, 24); sb.s(" loc="); sb.f(v[0], 1); sb.s(","); sb.f(v[1], 1); sb.s(","); sb.f(v[2], 1);
        RPM(root + oRelRot, v, 24); sb.s(" rot(P,Y,R)="); sb.f(v[0], 2); sb.s(","); sb.f(v[1], 2); sb.s(","); sb.f(v[2], 2); sb.s("\r\n");
        sb.s("--- pawn BP properties ---\r\n");
        g_dumpStop = "Pawn"; DumpStruct(sb, pawn, cls, 1, 0, false); g_dumpStop = 0;
        sb.s("--- BP components ---\r\n");
        char ty[64], cn[128];
        for (u64 s = cls; IsPtr(s); s = Super(s)) {
            if (!ObjName(s, cn, 128) || !sends(cn, "_C")) break;
            for (u64 f = FirstField(s); IsPtr(f); f = NextField(f)) {
                if (!FieldType(f, ty, 64) || !seq(ty, "ObjectProperty")) continue;
                u64 o = Rd<u64>(pawn + FieldOff(f)); if (!IsPtr(o)) continue;
                u64 oc = ObjClass(o); if (!ClassNameOf(o, cn, 128)) continue;
                if (!sends(cn, "_C") && !icontains(cn, "Movement") && !icontains(cn, "Material") && !icontains(cn, "PostProcess") && !icontains(cn, "Camera")) continue;
                FieldName(f, nm, 128); sb.s("* "); sb.s(nm); sb.s(" -> "); DumpObjRef(sb, o); sb.s("\r\n");
                if (sends(cn, "_C")) DumpStruct(sb, o, oc, 1, 1, true);
                else DumpStruct(sb, o, oc, (icontains(cn, "Material") || icontains(cn, "PostProcess") || icontains(cn, "Camera")) ? 3 : 0, 1, false);
                if (sb.n > DUMPCAP - 65536) break;
            }
        }
    }
    sb.s("--- user widgets (WBP_*) ---\r\n");
    char nm[128], cn[128], on[128];
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        if (!NameIdx(Rd<u32>(r.cls + 0x18), 0, cn, 128) || !sstarts(cn, "WBP_")) continue;
        NameIdx(r.name, r.num, nm, 128); if (sstarts(nm, "Default__")) continue;
        ObjName(r.outer, on, 128);
        sb.s("  "); sb.s(nm); sb.s(" ("); sb.s(cn); sb.s(") outer="); sb.s(on); sb.s(ListMatch(C.hudWidgets, cn, false) ? "   <== HUD target" : ""); sb.s("\r\n");
        if (sb.n > DUMPCAP - 65536) break;
    }
    FindHudInstances();
    for (i32 h = 0; h < g_hudN; h++) {
        sb.s("--- children of HUD "); DumpObjRef(sb, g_hudInst[h]); sb.s(" ---\r\n");
        for (u32 i = 0; i < g_objCount; i++) {
            ObjRec& r = g_objs[i];
            if (!IsUnder(r.ptr, g_hudInst[h])) continue;
            sb.s("    "); DumpObjRef(sb, r.ptr); sb.s(" outer="); ObjName(r.outer, on, 128); sb.s(on);
            u8 k = WidgetKind(r.cls);
            if (k & 2) { u64 stb = 0; sb.s(FindSharedPtr(r.ptr, PropsSize(cTextLayout), PropsSize(cTextBlock), &stb) ? " [STextBlock found]" : " [STextBlock NOT found]"); }
            sb.s("\r\n");
            if (sb.n > DUMPCAP - 65536) break;
        }
    }
    {
        u64 hud = CurrentHud(g_pawn, g_uiOff);
        sb.s("--- current drone HUD tree: "); DumpObjRef(sb, hud); sb.s(" slateVisOff="); sb.i(g_slateVis); sb.s(" ---\r\n");
        if (hud) {
            u64 ws[256]; i32 n = CollectTree(hud, ws, 256);
            u64 rs = SlateOf(hud); sb.s("    root slate="); sb.h(rs); if (rs && g_slateVis >= 0) { sb.s(" vis="); sb.h(Rd<u8>(rs + g_slateVis)); } sb.s("\r\n");
            for (i32 i = 0; i < n; i++) {
                sb.s("    "); DumpObjRef(sb, ws[i]); u64 sl = SlateOf(ws[i]); sb.s(" slate="); sb.h(sl);
                if (oWVis >= 0) { sb.s(" uvis="); sb.i(Rd<u8>(ws[i] + oWVis)); }
                if (sl && g_slateVis >= 0) { sb.s(" svis="); sb.h(Rd<u8>(sl + g_slateVis)); }
                sb.s("\r\n");
            }
        }
    }
    {
        sb.s("--- level actor classes ---\r\n");
        static u64 ck[4096]; static i32 cc[4096]; i32 cn2 = 0; char on2[128], cnm[128];
        for (u32 i = 0; i < g_objCount; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.outer, on2, 128) || !seq(on2, "PersistentLevel")) continue;
            i32 k = 0; for (; k < cn2; k++) if (ck[k] == r.cls) break;
            if (k == cn2) { if (cn2 >= 4096) continue; ck[cn2] = r.cls; cc[cn2] = 0; cn2++; }
            cc[k]++;
        }
        for (i32 k = 0; k < cn2; k++) { ObjName(ck[k], cnm, 128); sb.s("  "); sb.i(cc[k]); sb.s("x "); sb.s(cnm); sb.s("\r\n"); }
        sb.s("--- zone-like actors ---\r\n");
        i32 shown = 0;
        for (u32 i = 0; i < g_objCount && shown < 30; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.cls, cnm, 128)) continue;
            if (!(icontains(cnm, "Zone") || icontains(cnm, "EW") || icontains(cnm, "Jam") || icontains(cnm, "REB") || icontains(cnm, "Area") || icontains(cnm, "Signal") || icontains(cnm, "Noise") || icontains(cnm, "Interfer") || icontains(cnm, "Suppress"))) continue;
            if (!sends(cnm, "_C")) continue;
            shown++; sb.s("* "); DumpObjRef(sb, r.ptr); sb.s("\r\n"); DumpStruct(sb, r.ptr, r.cls, 2, 1, true);
            if (sb.n > DUMPCAP - 131072) break;
        }
        sb.s("--- save game ---\r\n");
        for (u32 i = 0; i < g_objCount; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.cls, cnm, 128) || !icontains(cnm, "SaveGame") || !sends(cnm, "_C")) continue;
            NameIdx(r.name, r.num, on2, 128); if (sstarts(on2, "Default__")) continue;
            sb.s("* "); DumpObjRef(sb, r.ptr); sb.s("\r\n"); DumpStruct(sb, r.ptr, r.cls, 2, 1, true);
            if (sb.n > DUMPCAP - 131072) break;
        }
    }
    {
        sb.s("--- foliage / cull volumes ---\r\n");
        i32 shown = 0; char cnm[128], on3[128];
        for (u32 i = 0; i < g_objCount && shown < 80; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.cls, cnm, 128)) continue;
            bool fol = icontains(cnm, "InstancedStaticMeshComponent");
            bool cdv = seq(cnm, "CullDistanceVolume") || seq(cnm, "ProceduralFoliageVolume");
            if (!fol && !cdv) continue;
            NameIdx(r.name, r.num, on3, 128); if (sstarts(on3, "Default__")) continue;
            shown++;
            sb.s("* "); DumpObjRef(sb, r.ptr); sb.s(" outer="); ObjName(r.outer, on3, 128); sb.s(on3); sb.s("\r\n");
            if (cdv) { DumpStruct(sb, r.ptr, r.cls, 2, 1, false); continue; }
            const char* pn[] = { "StaticMesh", "InstanceStartCullDistance", "InstanceEndCullDistance", "LDMaxDrawDistance", "CachedMaxDrawDistance", "CastShadow", "PerInstanceSMData", "MinLOD", "bUseAsOccluder" };
            for (i32 k = 0; k < 9; k++) {
                u64 f = FindField(r.cls, pn[k], true); if (!f) continue;
                char ty[64]; FieldType(f, ty, 64);
                sb.s("    "); sb.s(pn[k]); sb.s(" = "); DumpValue(sb, r.ptr + FieldOff(f), f, ty, 0, 2); sb.s("\r\n");
            }
            if (sb.n > DUMPCAP - 131072) break;
        }
        for (u32 i = 0; i < g_objCount; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.cls, cnm, 128) || !seq(cnm, "PostProcessVolume")) continue;
            sb.s("* "); DumpObjRef(sb, r.ptr); sb.s("\r\n");
            const char* pn[] = { "Priority", "BlendRadius", "BlendWeight", "bEnabled", "bUnbound" };
            for (i32 k = 0; k < 5; k++) { u64 f = FindField(r.cls, pn[k], true); if (!f) continue; char ty[64]; FieldType(f, ty, 64); sb.s("    "); sb.s(pn[k]); sb.s(" = "); DumpValue(sb, r.ptr + FieldOff(f), f, ty, 0, 2); sb.s("\r\n"); }
            u64 fs = FindField(r.cls, "Settings", true);
            if (fs) { u64 st = FieldExtra(fs); u64 fb = FindField(st, "WeightedBlendables", true); if (fb) { u64 bs = FieldExtra(fb); u64 fa = FindField(bs, "Array", true); if (fa) { sb.s("    blendables: "); DumpValue(sb, r.ptr + FieldOff(fs) + FieldOff(fb) + FieldOff(fa), fa, "ArrayProperty", 3, 2); sb.s("\r\n"); } } }
        }
    }
    if (seq(reason, "hotkey")) {
        sb.s("--- impulse calls in blueprint bytecode ---\r\n");
        const char* tn[] = { "AddImpulse", "AddImpulseAtLocation", "AddRadialImpulse", "AddForce", "AddForceAtLocation", "AddRadialForce", "AddAngularImpulseInDegrees", "AddAngularImpulseInRadians", "SetPhysicsLinearVelocity", "SetAllPhysicsLinearVelocity", "AddVelocityChangeImpulseAtLocation", "FireImpulse", "LaunchCharacter", "ApplyRadialDamage", "ApplyRadialDamageWithFalloff", "SetPhysicsAngularVelocityInDegrees", "AddTorqueInDegrees" };
        const i32 NT = sizeof(tn) / sizeof(tn[0]);
        static u64 tf[256]; static i32 tfi[256]; i32 ntf = 0; char fnm[128], cnm[128];
        for (u32 i = 0; i < g_objCount && ntf < 256; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.cls, cnm, 128) || !seq(cnm, "Function")) continue;
            if (!NameIdx(r.name, r.num, fnm, 128)) continue;
            for (i32 k = 0; k < NT; k++) if (seq(fnm, tn[k])) { tf[ntf] = r.ptr; tfi[ntf] = k; ntf++; break; }
        }
        sb.s("target functions found: "); sb.i(ntf); sb.s("\r\n");
        static u8* sbuf = 0; if (!sbuf) sbuf = (u8*)VirtualAlloc(0, 1 << 22, 0x3000, 4);
        i32 hits = 0;
        for (u32 i = 0; i < g_objCount && sbuf && hits < 200; i++) {
            ObjRec& r = g_objs[i];
            if (!ObjName(r.cls, cnm, 128) || !seq(cnm, "Function")) continue;
            if (!ObjName(r.outer, cnm, 128) || !sends(cnm, "_C")) continue;
            u64 sp = Rd<u64>(r.ptr + 0x60); i32 sn = Rd<i32>(r.ptr + 0x68);
            if (!IsPtr(sp) || sn <= 0 || sn > (1 << 22)) continue;
            if (!RPM(sp, sbuf, sn)) continue;
            for (i32 o = 0; o + 8 <= sn; o++) {
                u64 v = *(u64*)(sbuf + o); i32 k = -1;
                for (i32 q = 0; q < ntf; q++) if (tf[q] == v) { k = tfi[q]; break; }
                if (k < 0) continue;
                hits++;
                NameIdx(r.name, r.num, fnm, 128);
                sb.s("* "); sb.s(cnm); sb.s("."); sb.s(fnm); sb.s(" script="); sb.h(sp); sb.s(" off="); sb.h(o); sb.s(" -> "); sb.s(tn[k]); sb.s("\r\n    ");
                i32 a0 = o - 24 < 0 ? 0 : o - 24, a1 = o + 160 > sn ? sn : o + 160;
                for (i32 q = a0; q < a1; q++) { const char* hx = "0123456789ABCDEF"; if (q == o) sb.c('['); sb.c(hx[sbuf[q] >> 4]); sb.c(hx[sbuf[q] & 15]); if (q == o + 7) sb.c(']'); sb.c(' '); }
                sb.s("\r\n");
                if (sb.n > DUMPCAP - 131072) break;
            }
        }
    }
    sb.s("--- input mapping contexts ---\r\n");
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        if (!NameIdx(Rd<u32>(r.cls + 0x18), 0, cn, 128) || !seq(cn, "InputMappingContext")) continue;
        NameIdx(r.name, r.num, nm, 128);
        if (sstarts(nm, "Default__") || !(icontains(nm, "Kamikaze") || icontains(nm, "Drone") || icontains(nm, "Spectator") || icontains(nm, "Striker"))) continue;
        sb.s("IMC "); sb.s(nm); sb.s("\r\n");
        DumpStruct(sb, r.ptr, r.cls, 3, 1, false);
        if (sb.n > DUMPCAP - 65536) break;
    }
    {
        sb.s("--- vehicles / spawners / wreck candidates ---\r\n");
        i32 oLife = cActor ? PropOff(cActor, "InitialLifeSpan") : -1;
        static u64* hk = 0; static u8* hv = 0; const u32 HC = 1 << 17;
        if (!hk) { hk = (u64*)VirtualAlloc(0, (u64)HC * 8, 0x3000, 4); hv = (u8*)VirtualAlloc(0, HC, 0x3000, 4); }
        i32 shownV = 0, shownL = 0;
        static char lifeBuf[65536]; SB lb2{ lifeBuf, 0, sizeof(lifeBuf) };
        for (u32 i = 0; i < g_objCount; i++) {
            ObjRec& r = g_objs[i];
            u32 h = (u32)((r.cls >> 4) * 2654435761u) & (HC - 1);
            while (hk[h] && hk[h] != r.cls) h = (h + 1) & (HC - 1);
            if (!hk[h]) {
                u8 m = 0; char cnm[128];
                if (ChainHas(r.cls, "Actor")) {
                    m = 1;
                    for (u64 c = r.cls; IsPtr(c); c = Super(c)) { if (!ObjName(c, cnm, 128)) break; if (icontains(cnm, "Vehicle") || icontains(cnm, "Spawner") || icontains(cnm, "Wreck") || icontains(cnm, "BMP") || icontains(cnm, "Truck") || icontains(cnm, "Destroy") || icontains(cnm, "Debris")) { m = 2; break; } if (seq(cnm, "Actor")) break; }
                }
                hk[h] = r.cls; hv[h] = m;
            }
            u8 m = hv[h];
            if (!m) continue;
            float life = oLife >= 0 ? Rd<float>(r.ptr + oLife) : 0;
            if (life > 0.0f && shownL < 300) {
                shownL++; lb2.s("  life="); lb2.f(life, 2); lb2.s("  "); DumpObjRef(lb2, r.ptr); lb2.s("\r\n");
            }
            if (m != 2 || shownV >= 60) continue;
            shownV++;
            sb.s("* "); DumpObjRef(sb, r.ptr); sb.s(" InitialLifeSpan="); sb.f(life, 2); sb.s("\r\n");
            DumpStruct(sb, r.ptr, r.cls, 1, 1, true);
            if (sb.n > DUMPCAP - 131072) break;
        }
        lb2.z();
        sb.s("--- actors with InitialLifeSpan > 0 ---\r\n"); sb.s(lifeBuf);
    }
    sb.s("==================== END ====================\r\n");
    AppendFile(g_dumpPath, sb.p, sb.n);
    Log("dump written: ", reason);
}

static u64 g_vehBase = 0; static i32 g_oDestroyTime = -1;
static u64* g_vk = 0; static u8* g_vv = 0; static const u32 VHC = 1 << 16;
static void KeepWrecks() {
    if (!g_vehBase) {
        NameQ q[] = { { "BP_Vehicle_Base_C", "Class", 0 } };
        FindObjects(q, 1);
        if (!q[0].result) return;
        g_vehBase = q[0].result; g_oDestroyTime = PropOff(g_vehBase, "DestroyTime");
        static char B_[128]; SB sb{ B_, 0, 128 }; sb.s("wrecks: BP_Vehicle_Base_C found, DestroyTime @"); sb.h(g_oDestroyTime); sb.z(); Log(B_);
    }
    if (g_oDestroyTime < 0) return;
    if (!g_vk) { g_vk = (u64*)VirtualAlloc(0, (u64)VHC * 8, 0x3000, 4); g_vv = (u8*)VirtualAlloc(0, VHC, 0x3000, 4); if (!g_vk || !g_vv) return; }
    i32 patched = 0;
    const float big = 100000000.0f;
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        u32 h = (u32)((r.cls >> 4) * 2654435761u) & (VHC - 1);
        i32 guard = 0;
        while (g_vk[h] && g_vk[h] != r.cls && guard++ < 64) h = (h + 1) & (VHC - 1);
        if (g_vk[h] != r.cls) {
            if (g_vk[h]) continue;
            u8 m = 0; u64 c = r.cls;
            for (i32 d = 0; d < 24 && IsPtr(c); d++) { if (c == g_vehBase) { m = 1; break; } c = Super(c); }
            g_vk[h] = r.cls; g_vv[h] = m;
        }
        if (!g_vv[h]) continue;
        float v = Rd<float>(r.ptr + g_oDestroyTime);
        if (v > 0.0f && v < 10000000.0f && ObjLive(r.gidx, r.ptr, r.cls)) { if (WPM(r.ptr + g_oDestroyTime, &big, 4)) patched++; }
    }
    if (patched) { static char B_[128]; SB sb{ B_, 0, 128 }; sb.s("wrecks: DestroyTime set to forever on "); sb.i(patched); sb.s(" vehicle object(s)"); sb.z(); Log(B_); }
}

struct KbInfo { u8 n; i32 off[6]; u8 dbl[6]; };
static u64* g_kbK = 0; static KbInfo* g_kbV = 0; static const u32 KBC = 1 << 15;
struct KbPatch { u64 addr; double orig; u8 dbl; };
static KbPatch g_kbP[8192]; static i32 g_kbPN = 0; static i32 g_kbLogged = 0;
static void KbClassInfo(u64 cls, KbInfo* out) {
    out->n = 0;
    bool isDmg = ChainHas(cls, "DamageType"), isRad = ChainHas(cls, "RadialForceComponent");
    bool isFS = ChainHas(cls, "FS_MasterField_C") || ChainHas(cls, "FieldSystemActor");
    char cn[128], nm[128], ty[64];
    for (u64 s2 = cls; IsPtr(s2) && out->n < 6; s2 = Super(s2)) {
        if (!ObjName(s2, cn, 128)) break;
        bool bp = sends(cn, "_C");
        for (u64 f = FirstField(s2); IsPtr(f) && out->n < 6; f = NextField(f)) {
            if (!FieldType(f, ty, 64)) continue;
            bool d = seq(ty, "DoubleProperty"); if (!d && !seq(ty, "FloatProperty")) continue;
            if (!FieldName(f, nm, 128)) continue;
            bool hit = false;
            if (isDmg && (seq(nm, "DamageImpulse") || seq(nm, "DestructibleImpulse"))) hit = true;
            if (isRad && (seq(nm, "ImpulseStrength") || seq(nm, "ForceStrength"))) hit = true;
            if (bp && (icontains(nm, "Impulse") || icontains(nm, "Knockback") || icontains(nm, "PushForce") || icontains(nm, "ExplosionForce") || icontains(nm, "RadialForce"))) hit = true;
            if (isFS && bp && (icontains(nm, "Force") || icontains(nm, "Vel") || icontains(nm, "Torque") || icontains(nm, "Impulse") || icontains(nm, "Push") || icontains(nm, "Magnitude"))
                && !icontains(nm, "Strain") && !icontains(nm, "Damage") && !icontains(nm, "Anchor") && !icontains(nm, "Radius") && !icontains(nm, "Falloff") && !icontains(nm, "Threshold") && !icontains(nm, "Kill") && !icontains(nm, "Sleep") && !icontains(nm, "Disable")) hit = true;
            if (!hit) continue;
            out->off[out->n] = FieldOff(f); out->dbl[out->n] = d ? 1 : 0; out->n++;
            if (g_kbLogged < 120) { g_kbLogged++; static char B_[256]; SB sb{ B_, 0, 256 }; sb.s("knockback field: "); sb.s(cn); sb.s("."); sb.s(nm); sb.z(); Log(B_); }
        }
        if (seq(cn, "Object")) break;
    }
}
static void Knockback() {
    if (!g_kbK) { g_kbK = (u64*)VirtualAlloc(0, (u64)KBC * 8, 0x3000, 4); g_kbV = (KbInfo*)VirtualAlloc(0, (u64)KBC * sizeof(KbInfo), 0x3000, 4); if (!g_kbK || !g_kbV) return; }
    i32 changed = 0;
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        u32 h = (u32)((r.cls >> 4) * 2654435761u) & (KBC - 1); i32 guard = 0;
        while (g_kbK[h] && g_kbK[h] != r.cls && guard++ < 64) h = (h + 1) & (KBC - 1);
        if (g_kbK[h] != r.cls) { if (g_kbK[h]) continue; g_kbK[h] = r.cls; KbClassInfo(r.cls, &g_kbV[h]); }
        KbInfo& ki = g_kbV[h]; if (!ki.n) continue;
        for (i32 k = 0; k < ki.n; k++) {
            u64 a = r.ptr + ki.off[k];
            double cur = ki.dbl[k] ? Rd<double>(a) : (double)Rd<float>(a);
            i32 p = -1; for (i32 q = 0; q < g_kbPN; q++) if (g_kbP[q].addr == a) { p = q; break; }
            double orig;
            if (p < 0) { if (g_kbPN >= 8192) continue; p = g_kbPN++; g_kbP[p].addr = a; g_kbP[p].orig = cur; g_kbP[p].dbl = ki.dbl[k]; orig = cur; }
            else orig = g_kbP[p].orig;
            double want = orig * C.knockScale;
            if (cur == want) continue;
            if (cur != orig) { g_kbP[p].orig = cur; want = cur * C.knockScale; }
            if (!ObjLive(r.gidx, r.ptr, r.cls)) break;
            if (ki.dbl[k]) WPM(a, &want, 8); else { float fw = (float)want; WPM(a, &fw, 4); }
            changed++;
        }
    }
    if (changed) { static char B_[96]; SB sb{ B_, 0, 96 }; sb.s("knockback: scaled "); sb.i(changed); sb.s(" value(s)"); sb.z(); Log(B_); }
}
static void FindLandscape() {
    char cn[128], on[128];
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        if (!ObjName(r.cls, cn, 128) || !(seq(cn, "Landscape") || seq(cn, "LandscapeStreamingProxy"))) continue;
        NameIdx(r.name, r.num, on, 128); if (sstarts(on, "Default__")) continue;
        u64 root = Rd<u64>(r.ptr + oRoot); if (!IsPtr(root)) continue;
        double z = Rd<double>(root + oRelLoc + 16);
        g_landZ = z; g_landOk = 1;
        static char B_[96]; SB sb{ B_, 0, 96 }; sb.s("altitude: sea level = landscape Z "); sb.f(z / 100.0, 1); sb.s(" m"); sb.z(); Log(B_);
        return;
    }
}
struct FolPatch { u64 p, cls; u32 gidx; i32 s0, e0; bool seen; };
static FolPatch g_fol[4096]; static i32 g_folN = 0; static i32 oFolStart = -1, oFolEnd = -1; static u64 g_folCls[8]; static i32 g_folClsN = 0;
static void FolTarget(const FolPatch& f, bool on, i32& ws, i32& we) {
    double e = f.e0 * C.folCull; if (e < C.folMin) e = C.folMin;
    double st = e * C.folFade; if (st < f.s0 * C.folCull) st = f.s0 * C.folCull; if (st > e) st = e;
    ws = on ? (i32)st : f.s0; we = on ? (i32)e : f.e0;
}
static void FoliageCull(bool on) {
    char cn[128];
    i32 added = 0;
    for (i32 k = 0; k < g_folN; k++) g_fol[k].seen = false;
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        bool known = false; for (i32 k = 0; k < g_folClsN; k++) if (g_folCls[k] == r.cls) known = true;
        if (!known) {
            if (!ObjName(r.cls, cn, 128) || !(seq(cn, "FoliageInstancedStaticMeshComponent") || seq(cn, "HierarchicalInstancedStaticMeshComponent") || seq(cn, "GrassInstancedStaticMeshComponent"))) continue;
            if (g_folClsN < 8) g_folCls[g_folClsN++] = r.cls;
            if (oFolStart < 0) { oFolStart = PropOff(r.cls, "InstanceStartCullDistance"); oFolEnd = PropOff(r.cls, "InstanceEndCullDistance"); }
        }
        if (oFolStart < 0 || oFolEnd < 0) return;
        i32 idx = -1; for (i32 k = 0; k < g_folN; k++) if (g_fol[k].p == r.ptr && g_fol[k].gidx == r.gidx && g_fol[k].cls == r.cls) { idx = k; break; }
        if (idx < 0) {
            if (g_folN >= 4096) continue;
            i32 s0 = Rd<i32>(r.ptr + oFolStart), e0 = Rd<i32>(r.ptr + oFolEnd);
            if (e0 <= 0 || e0 > 10000000 || s0 < 0 || s0 > e0) continue;
            FolPatch& n = g_fol[g_folN]; n.p = r.ptr; n.cls = r.cls; n.gidx = r.gidx; n.s0 = s0; n.e0 = e0; idx = g_folN++; added++;
        }
        g_fol[idx].seen = true;
    }
    i32 w = 0, live = 0, skipDead = 0, skipVal = 0;
    for (i32 k = 0; k < g_folN; k++) {
        FolPatch f = g_fol[k];
        if (!f.seen) continue;
        g_fol[w++] = f;
        i32 onS, onE, offS, offE; FolTarget(f, true, onS, onE); FolTarget(f, false, offS, offE);
        i32 ws = on ? onS : offS, we = on ? onE : offE;
        if (!ObjLive(f.gidx, f.p, f.cls)) { skipDead++; continue; }
        i32 cs = Rd<i32>(f.p + oFolStart), ce = Rd<i32>(f.p + oFolEnd);
        if (!((cs == onS || cs == offS) && (ce == onE || ce == offE))) { skipVal++; continue; }
        live++;
        if (cs != ws) WPM(f.p + oFolStart, &ws, 4);
        if (ce != we) WPM(f.p + oFolEnd, &we, 4);
    }
    g_folN = w;
    if (added) { static char B_[128]; SB sb{ B_, 0, 128 }; sb.s("foliage: cull end >= "); sb.f(C.folMin / 100.0, 0); sb.s(" m, fade from "); sb.f(C.folFade * 100, 0); sb.s("% new="); sb.i(added); sb.s(" live="); sb.i(live); sb.s(" skipDead="); sb.i(skipDead); sb.s(" skipVal="); sb.i(skipVal); sb.z(); Log(B_); }
}
struct EwVol { u64 p, cls; u32 gidx; float orig; bool saved; };
static EwVol g_ew[64]; static i32 g_ewN = 0; static bool g_ewBuilt = false; static i32 oPPVWeight = -1;
static bool g_naniteLogged = false;
static void LogFoliageNanite() {
    g_naniteLogged = true;
    char cn[128], mn[128];
    for (u32 i = 0; i < g_objCount; i++) {
        ObjRec& r = g_objs[i];
        if (!ObjName(r.cls, cn, 128) || !seq(cn, "FoliageInstancedStaticMeshComponent")) continue;
        u64 fsm = FindField(r.cls, "StaticMesh", true); if (!fsm) return;
        u64 mesh = Rd<u64>(r.ptr + FieldOff(fsm)); if (!IsPtr(mesh)) continue;
        ObjName(mesh, mn, 128); if (!icontains(mn, "tree")) continue;
        u64 fn = FindField(ObjClass(mesh), "NaniteSettings", true);
        static char B_[256]; SB sb{ B_, 0, 256 }; sb.s("foliage mesh "); sb.s(mn);
        if (fn) {
            u64 fe = FindField(FieldExtra(fn), "bEnabled", true);
            if (fe) { u8 bo = Rd<u8>(fe + L.pExtra + 1), bm = Rd<u8>(fe + L.pExtra + 2); bool en = (Rd<u8>(mesh + FieldOff(fn) + FieldOff(fe) + bo) & bm) != 0; sb.s(en ? ": Nanite ON" : ": Nanite off (classic LODs)"); }
        } else sb.s(": NaniteSettings not reflected");
        sb.z(); Log(B_); return;
    }
}
static void BuildEwList() {
    if (!g_naniteLogged) LogFoliageNanite();
    g_ewN = 0; g_ewBuilt = true;
    char cn[128], on[128];
    i32 oSet = -1, oWB = -1, oArr = -1;
    for (u32 i = 0; i < g_objCount && g_ewN < 64; i++) {
        ObjRec& r = g_objs[i];
        if (!ObjName(r.cls, cn, 128) || !seq(cn, "PostProcessVolume")) continue;
        NameIdx(r.name, r.num, on, 128); if (sstarts(on, "Default__")) continue;
        if (oSet < 0) {
            u64 fs = FindField(r.cls, "Settings", true); if (!fs) return;
            u64 fb = FindField(FieldExtra(fs), "WeightedBlendables", true); if (!fb) return;
            u64 fa = FindField(FieldExtra(fb), "Array", true); if (!fa) return;
            oSet = FieldOff(fs); oWB = FieldOff(fb); oArr = FieldOff(fa);
            oPPVWeight = PropOff(r.cls, "BlendWeight"); if (oPPVWeight < 0) return;
        }
        u64 arr = Rd<u64>(r.ptr + oSet + oWB + oArr); i32 n = Rd<i32>(r.ptr + oSet + oWB + oArr + 8);
        if (!IsPtr(arr) || n <= 0 || n > 16) continue;
        bool ew = false;
        for (i32 k = 0; k < n; k++) { u64 o = Rd<u64>(arr + (u64)k * 16 + 8); if (ObjName(o, on, 128) && icontains(on, "NoSignal")) ew = true; }
        if (!ew) continue;
        g_ew[g_ewN].p = r.ptr; g_ew[g_ewN].cls = r.cls; g_ew[g_ewN].gidx = r.gidx; g_ew[g_ewN].saved = false; g_ewN++;
    }
    if (!g_ewN) return;
    static char B_[96]; SB sb{ B_, 0, 96 }; sb.s("EW: found "); sb.i(g_ewN); sb.s(" jamming volume(s)"); sb.z(); Log(B_);
}
static void ApplyEw(bool on) {
    for (i32 i = 0; i < g_ewN; i++) {
        EwVol& v = g_ew[i];
        if (!ObjLive(v.gidx, v.p, v.cls)) continue;
        float cur = Rd<float>(v.p + oPPVWeight);
        if (on) { if (cur != 0.0f) { if (!v.saved) { v.orig = cur; v.saved = true; } float z = 0; WPM(v.p + oPPVWeight, &z, 4); } }
        else if (v.saved) { if (cur == 0.0f) WPM(v.p + oPPVWeight, &v.orig, 4); v.saved = false; }
    }
}


static volatile i32 g_fpsShown = 0; static u64 g_fpsAddr = 0;
static u32 g_fpsC[1024]; static i32 g_fpsCN = -1;
static void FpsCollect() {
    g_fpsCN = 0;
    i32 pe = Rd<i32>(g_base + 0x3C); u16 ns = Rd<u16>(g_base + pe + 6), osz = Rd<u16>(g_base + pe + 20);
    u64 tR = 0, tS = 0, dR = 0, dS = 0;
    for (i32 i = 0; i < ns; i++) { u64 sh = g_base + pe + 24 + osz + (u64)i * 40; char nm[9]; RPM(sh, nm, 8); nm[8] = 0; if (seq(nm, ".text")) { tS = Rd<u32>(sh + 8); tR = Rd<u32>(sh + 12); } if (seq(nm, ".data")) { dS = Rd<u32>(sh + 8); dR = Rd<u32>(sh + 12); } }
    if (!tR || !dR) return;
    const u64 CH_ = 0x100000; u8* b = (u8*)VirtualAlloc(0, CH_ + 16, 0x3000, 4); if (!b) return;
    for (u64 o = 0; o < tS && g_fpsCN < 1024; o += CH_ - 8) {
        u64 n = tS - o < CH_ ? tS - o : CH_;
        if (!RPM(g_base + tR + o, b, n)) { memset(b, 0, n); for (u64 q = 0; q < n; q += 0x1000) RPM(g_base + tR + o + q, b + q, n - q < 0x1000 ? n - q : 0x1000); }
        for (u64 k = 1; k + 7 <= n; k++) {
            if (b[k] != 0xFF || b[k + 1] != 0x05) continue;
            bool q64 = b[k - 1] == 0x48; i32 L = 6;
            if (!q64 && (b[k - 1] == 0x49 || b[k - 1] == 0x4C || b[k - 1] == 0x41 || b[k - 1] == 0x40)) continue;
            i32 disp = *(i32*)(b + k + 2); u64 t = tR + o + k + L + (i64)disp;
            if (t < dR || t + 4 > dR + dS) continue;
            bool dup = false; for (i32 z = 0; z < g_fpsCN; z++) if (g_fpsC[z] == (u32)t) { dup = true; break; }
            if (!dup && g_fpsCN < 1024) g_fpsC[g_fpsCN++] = (u32)t;
        }
    }
    VirtualFree(b, 0, 0x8000);
    static char B_[64]; SB sb{ B_, 0, 64 }; sb.s("fps: inc targets="); sb.i(g_fpsCN); sb.z(); Log(B_);
}
static void FpsTick(u64 now) {
    static u64 lastT = 0, nextTry = 0, tPrev = 0; static u32 lastVal = 0; static double smooth = 0; static i32 stall = 0;
    static u32 pv[1024]; static u32 d0[1024]; static u8 ok[1024]; static i32 round = -1;
    if (g_fpsAddr) {
        if (now - lastT < 500) return;
        u32 v = Rd<u32>(g_fpsAddr);
        if (lastT) {
            u32 d = v - lastVal; double f = d * 1000.0 / (double)(now - lastT); if (f > 1000) f = 0;
            smooth = smooth <= 0 ? f : smooth * 0.5 + f * 0.5; g_fpsShown = (i32)(smooth + 0.5);
            if (d == 0) { if (++stall >= 20) { g_fpsAddr = 0; stall = 0; round = -1; nextTry = now; Log("fps: counter stalled, searching again"); } } else stall = 0;
        }
        lastVal = v; lastT = now; return;
    }
    if (g_fpsCN < 0) FpsCollect();
    if (g_fpsCN <= 0 || now < nextTry) return;
    if (round < 0) { for (i32 k = 0; k < g_fpsCN; k++) { pv[k] = Rd<u32>(g_base + g_fpsC[k]); ok[k] = 1; d0[k] = 0; } tPrev = now; round = 0; return; }
    if (now - tPrev < 500) return;
    double dt = (double)(now - tPrev); tPrev = now;
    for (i32 k = 0; k < g_fpsCN; k++) {
        u32 v = Rd<u32>(g_base + g_fpsC[k]); u32 d = v - pv[k]; pv[k] = v;
        if (!ok[k]) continue;
        double rate = d * 1000.0 / dt;
        if (rate < 5 || rate > 700) { ok[k] = 0; continue; }
        if (round == 0) d0[k] = d; else { double r = (double)d / (double)d0[k]; if (r < 0.33 || r > 3.0) ok[k] = 0; }
    }
    round++;
    if (round < 4) return;
    i32 best = -1, bestN = 0;
    for (i32 k = 0; k < g_fpsCN; k++) { if (!ok[k]) continue; i32 c = 0; for (i32 q = 0; q < g_fpsCN; q++) if (ok[q] && d0[q] * 100 >= d0[k] * 90 && d0[q] * 100 <= d0[k] * 110) c++; if (c > bestN || (c == bestN && best >= 0 && d0[k] > d0[best])) { bestN = c; best = k; } }
    i32 alive = 0; for (i32 k = 0; k < g_fpsCN; k++) alive += ok[k];
    static char B_[128]; SB sb{ B_, 0, 128 };
    if (best >= 0) { g_fpsAddr = g_base + g_fpsC[best]; lastT = 0; smooth = 0; sb.s("fps counter: rva="); sb.h(g_fpsC[best]); sb.s(" alive="); sb.i(alive); sb.s(" group="); sb.i(bestN); sb.s(" ~fps="); sb.i((i64)(d0[best] * 1000.0 / dt)); }
    else { sb.s("fps counter: none this pass, retry in 5 s"); nextTry = now + 5000; }
    sb.z(); Log(B_); round = -1;
}
static u32 __stdcall Worker(void*) {
    u64 lastInit = 0, lastHud = 0; i32 lastSerial = -1; u64 dumpAt = 0; i32 initTries = 0; u64 hudPawn = 0;
    u64 lastWorld = 0, wreckAt = 0;
    for (;;) {
        Sleep(20);
        if (!g_proc || !g_base) continue;
        u64 now = GetTickCount64();
        if (!g_reflReady) {
            if (now - lastInit < 3000) continue;
            lastInit = now; initTries++;
            if (InitReflection()) { g_reflReady = 1; Log("reflection ready"); if (!g_cvTried) { g_cvTried = true; CreateThread(0, 0, SPThread, 0, 0, 0); } }
            else if (initTries == 20) Log("reflection still not ready after 20 tries (game still loading?)");
            continue;
        }
        if (g_pawnSerial != lastSerial) { lastSerial = g_pawnSerial; if (g_pawn) dumpAt = now + 2500; }
        if (dumpAt && now >= dumpAt) { dumpAt = 0; DoDump("pawn changed"); }
        if (g_dumpReq) { g_dumpReq = 0; DoDump("hotkey"); }
        if (C.noNoise && g_kamikazeActive) KillInterference(g_pawn);
        if (g_ewBuilt) ApplyEw(C.noNoise && g_kamikazeActive && !g_userHidden);
        ApplyScreenPercentage(g_kamikazeActive && C.camHeight > 0 && !g_userHidden, g_winH);
        FpsTick(now);
        {
            u64 wld = Rd<u64>(g_base + RVA_GWORLD);
            if (wld != lastWorld) { lastWorld = wld; wreckAt = now + 3000; g_ewBuilt = false; g_ewN = 0; g_naniteLogged = false; g_landOk = 0; g_folN = 0; g_folClsN = 0; g_kbPN = 0; if (g_kbK) memset(g_kbK, 0, (u64)KBC * 8); g_vehBase = 0; g_oDestroyTime = -1; if (g_vk) { memset(g_vk, 0, (u64)VHC * 8); } }
            if (IsPtr(wld) && now >= wreckAt) { wreckAt = now + 10000; if (ScanObjects()) { if (!g_landOk) FindLandscape(); if (C.keepWrecks) KeepWrecks(); if (C.knockScale < 0.999) Knockback(); if (!g_ewBuilt || g_ewN == 0) BuildEwList(); } }
        }
        u64 pawn = g_pawn;
        if (pawn != hudPawn) { hudPawn = pawn; g_vsN = 0; g_hudCur = 0; }
        bool want = C.hideHud && g_kamikazeActive && !g_userHidden;
        if (now - lastHud < 60) continue;
        lastHud = now;
        u64 hud = CurrentHud(pawn, g_uiOff);
        if (hud != g_hudCur) { g_vsN = 0; g_hudCur = hud; if (hud) Log("game HUD bound to current drone"); }
        if (!hud) continue;
        if (want) HideHud(hud);
        else if (g_vsN) RestoreHud();
    }
    return 0;
}
static u32 g_targetPid = 0;
static i32 __stdcall EnumProc(HWND h, i64 lp) {
    u32 pid = 0; GetWindowThreadProcessId(h, &pid);
    if (pid != g_targetPid || !IsWindowVisible(h)) return 1;
    RECT rc; GetClientRect(h, &rc);
    HWND* best = (HWND*)lp;
    if (rc.r > 400 && rc.b > 300) {
        if (!*best) *best = h;
        else { RECT rb; GetClientRect(*best, &rb); if ((i64)rc.r * rc.b > (i64)rb.r * rb.b) *best = h; }
    }
    return 1;
}
static u32 FindGamePid() {
    HANDLE s = CreateToolhelp32Snapshot(2, 0); if (s == (HANDLE)-1) return 0;
    PROCESSENTRY32W pe; memset(&pe, 0, sizeof(pe)); pe.dwSize = sizeof(pe); u32 pid = 0;
    if (Process32FirstW(s, &pe)) do {
        const wc* a = pe.exe; const wc* b = L"FPVKamikazeDrone-Win64-Shipping.exe"; bool eq = true;
        while (*a || *b) { wc x = *a, y = *b; if (x >= 'A' && x <= 'Z') x += 32; if (y >= 'A' && y <= 'Z') y += 32; if (x != y) { eq = false; break; } a++; b++; }
        if (eq) { pid = pe.pid; break; }
    } while (Process32NextW(s, &pe));
    CloseHandle(s); return pid;
}
static bool AttachProcess(u32 pid) {
    g_proc = OpenProcess(0x10 | 0x20 | 0x08 | 0x400 | 0x100000, 0, pid);
    if (!g_proc) g_proc = OpenProcess(0x10 | 0x400 | 0x100000, 0, pid);
    if (!g_proc) { Log("OpenProcess failed (run overlay as admin if game runs as admin)"); return false; }
    void* mods[4]; u32 need = 0;
    if (!K32EnumProcessModules(g_proc, mods, sizeof(mods), &need) || !mods[0]) { Log("EnumProcessModules failed"); CloseHandle(g_proc); g_proc = 0; return false; }
    g_base = (u64)mods[0];
    i32 pe = Rd<i32>(g_base + 0x3C); g_imgSize = Rd<u32>(g_base + pe + 0x50);
    g_pid = pid;
    static char B_[2048]; SB sb{ B_, 0, 2048 }; sb.s("attached pid="); sb.i(pid); sb.s(" base="); sb.h(g_base); sb.s(" size="); sb.h(g_imgSize);
    if (g_imgSize != 0xAC6B000) sb.s("  WARNING: exe differs from the one offsets were taken from");
    sb.z(); Log(B_);
    return true;
}

static void* g_brCross[4] = { 0, 0, 0, 0 }; static void* g_brDot = 0; static void* g_penW = 0; static void* g_penB = 0;
static void* g_fontS = 0; static void* g_fontT = 0; static void* g_brG = 0; static void* g_brR = 0; static void* g_brGr = 0;
static void* g_gpCanvas = 0; static void* g_gpG = 0; static void* g_gpTpl = 0; static void* g_font = 0; static void* g_brW = 0; static void* g_brB = 0; static void* g_fmtL = 0; static void* g_fmtR = 0;
static u8* g_canvas = 0;
static HDC g_memDC = 0; static HANDLE g_dib = 0; static u32* g_dibBits = 0; static i32 g_dibW = 0, g_dibH = 0;
static i32 g_xmap[8192];

static bool InitRender() {
    GdiplusStartupInput in = { 1, 0, 0, 0 }; u64 tok = 0;
    if (GdiplusStartup(&tok, &in, 0) != 0) return false;
    wc path[600]; wcpy(path, g_dir, 600); wcat(path, L"osd_template.png", 600);
    if (GdipCreateBitmapFromFile(path, &g_gpTpl) != 0) { g_gpTpl = 0; Log("osd_template.png not found next to exe"); return false; }
    g_canvas = (u8*)VirtualAlloc(0, (u64)CW * CH * 4, 0x3000, 4);
    if (GdipCreateBitmapFromScan0(CW, CH, CW * 4, 0x000E200B, g_canvas, &g_gpCanvas) != 0) return false;
    if (GdipGetImageGraphicsContext(g_gpCanvas, &g_gpG) != 0) return false;
    GdipSetInterpolationMode(g_gpG, 5); GdipSetPixelOffsetMode(g_gpG, 4); GdipSetTextRenderingHint(g_gpG, 4); GdipSetSmoothingMode(g_gpG, 4);
    void* fam = 0;
    if (GdipCreateFontFamilyFromName(C.font, 0, &fam) != 0) { Log("font not found, using generic serif"); GdipGetGenericFontFamilySerif(&fam); }
    if (GdipCreateFont(fam, (float)C.fontSize, C.fontBold ? 1 : 0, 2, &g_font) != 0) return false;
    GdipCreateFont(fam, (float)(C.fontSize * 0.80), C.fontBold ? 1 : 0, 2, &g_fontS);
    GdipCreateFont(fam, (float)(C.fontSize * 0.72), C.fontBold ? 1 : 0, 2, &g_fontT);
    GdipCreateSolidFill(0xFF54A33E, &g_brG); GdipCreateSolidFill(0xFFA81324, &g_brR); GdipCreateSolidFill(0xFF858585, &g_brGr);
    GdipCreateSolidFill(0xFFF2F2F2, &g_brW); GdipCreateSolidFill(0xF0101010, &g_brB);
    GdipCreateSolidFill(0xFFF4F4F4, &g_brCross[0]); GdipCreateSolidFill(0xFF1EE01E, &g_brCross[1]); GdipCreateSolidFill(0xFFFFE21A, &g_brCross[2]); GdipCreateSolidFill(0xFFFF1E1E, &g_brCross[3]);
    GdipCreateSolidFill(0xFFF4F4F4, &g_brDot);
    GdipCreatePen1(0xFFF4F4F4, 2.2f, 2, &g_penW); GdipCreatePen1(0xF0101010, 4.2f, 2, &g_penB);
    GdipSetPenLineCap197819(g_penW, 2, 2, 0); GdipSetPenLineCap197819(g_penB, 2, 2, 0);
    GdipCreateStringFormat(0x1000 | 0x4000, 0, &g_fmtL); GdipCreateStringFormat(0x1000 | 0x4000, 0, &g_fmtR); GdipSetStringFormatAlign(g_fmtR, 2);
    return true;
}
static void EnsureDib(i32 w, i32 h) {
    if (w == g_dibW && h == g_dibH && g_dib) return;
    if (g_memDC) DeleteDC(g_memDC); if (g_dib) DeleteObject(g_dib); g_memDC = 0; g_dib = 0;
    HDC scr = GetDC(0); g_memDC = CreateCompatibleDC(scr); ReleaseDC(0, scr);
    BITMAPINFO bi; memset(&bi, 0, sizeof(bi)); bi.h.size = sizeof(BITMAPINFOHEADER); bi.h.w = w; bi.h.h = -h; bi.h.planes = 1; bi.h.bits = 32;
    void* bits = 0; g_dib = CreateDIBSection(g_memDC, &bi, 0, &bits, 0, 0); g_dibBits = (u32*)bits; SelectObject(g_memDC, g_dib);
    g_dibW = w; g_dibH = h;
    for (i32 x = 0; x < w && x < 8192; x++) g_xmap[x] = (i32)((i64)x * CW / w);
}
static void TextEx(const wc* s, float x, float y, bool right, void* font, void* brush) {
    i32 n = wlen(s);
    RectF r = right ? RectF{ x - 260.0f, y, 260.0f, 30.0f } : RectF{ x, y, 300.0f, 30.0f };
    void* fmt = right ? g_fmtR : g_fmtL;
    for (i32 dy = -1; dy <= 1; dy++) for (i32 dx = -1; dx <= 1; dx++) { if (!dx && !dy) continue; RectF o = { r.x + dx, r.y + dy, r.w, r.h }; GdipDrawString(g_gpG, s, n, font, &o, fmt, g_brB); }
    GdipDrawString(g_gpG, s, n, font, &r, fmt, brush);
}
static void Text(const wc* s, float x, float y, bool right) { TextEx(s, x, y, right, g_font, g_brW); }
static void DrawStaticLabels() {
    void* F = g_font; void* S = g_fontS; void* T = g_fontT;
    TextEx(L"ACRO", 13, 16, false, F, g_brG);
    TextEx(L"pitch", 682, 41, true, F, g_brW);
    TextEx(L"alt m", 89, 81, false, F, g_brW);
    TextEx(L"\x0417 \x0410   \x0423 \x0420 \x0410 \x041B \x0410", 25, 103, false, T, g_brG);
    TextEx(L"0", 16, 268, false, F, g_brW);
    TextEx(L"\x042D. \x043F\x0440\x0435\x0434.:  \x0432\x044B\x043A\x043B", 16, 297, false, F, g_brR);
    TextEx(L"\x0427\x0435\x043A\x0430: \x043D\x0435\x0442", 17, 327, false, F, g_brR);
    TextEx(L"link-ok", 16, 357, false, F, g_brW);
    TextEx(L"RXg:", 71, 409, true, F, g_brW);
    TextEx(L"TXg:", 67, 444, true, F, g_brW);
    TextEx(L"A", 888, 326, false, F, g_brW);
    TextEx(L"mAh", 914, 356, false, F, g_brW);
    TextEx(L"\x0433\x0430\x0437 :", 887, 409, true, F, g_brW);
    TextEx(L"%", 924, 409, false, F, g_brW);
    TextEx(L"FW: 2.27.3", 15, 531, false, F, g_brW);
    TextEx(L"SN: 637738473", 799, 531, false, F, g_brW);
    TextEx(L"\x0411\x041F\x041B\x0410", 13, 558, false, S, g_brG);
    TextEx(L"\x041F\x0443\x043B\x044C\x0442", 73, 558, false, S, g_brG);
    TextEx(L"\x041D\x0421\x0423", 143, 558, false, S, g_brG);
    TextEx(L"\x0417\x0430\x043F\x0438\x0441\x044C", 195, 558, false, S, g_brG);
    TextEx(L"\x0412\x043A\x043B.\x0442\x0435\x043F\x043B.", 280, 558, false, S, g_brG);
    TextEx(L"\x0421\x0442\x0430\x043D\x0434", 403, 558, false, S, g_brG);
    TextEx(L"\x0422\x0435\x043F\x043B \x0437\x0430\x0433\x0440.", 489, 558, false, S, g_brGr);
}
static void Upscale() {
    for (i32 y = 0; y < g_dibH; y++) {
        const u32* src = (const u32*)(g_canvas + (u64)((i64)y * CH / g_dibH) * CW * 4);
        u32* dst = g_dibBits + (u64)y * g_dibW;
        for (i32 x = 0; x < g_dibW; x++) dst[x] = src[g_xmap[x]];
    }
}

static i32 FmtW(wc* out, double v, i32 dec, bool plus, const wc* suffix) {
    char b[48]; SB sb{ b, 0, 48 };
    if (plus && v >= 0) sb.c('+');
    sb.f(v, dec); sb.z();
    i32 n = 0; for (; b[n]; n++) out[n] = b[n];
    if (suffix) for (i32 k = 0; suffix[k]; k++) out[n++] = suffix[k];
    out[n] = 0; return n;
}

static const double PI = 3.14159265358979323846;
static double Sin(double x) {
    if (x > 1e6 || x < -1e6 || x != x) return 0;
    while (x > PI) x -= 2 * PI; while (x < -PI) x += 2 * PI;
    if (x > PI / 2) x = PI - x; else if (x < -PI / 2) x = -PI - x;
    double x2 = x * x;
    return x * (1 - x2 / 6 * (1 - x2 / 20 * (1 - x2 / 42 * (1 - x2 / 72 * (1 - x2 / 110 * (1 - x2 / 156))))));
}
static double Cos(double x) { return Sin(x + PI / 2); }
static double Sqrt(double v) { if (!(v > 0)) return 0; double r = v > 1 ? v * 0.5 : 1.0; for (i32 i = 0; i < 60; i++) { double n = 0.5 * (r + v / r); if (n == r) break; r = n; } return r; }
struct V3 { double x, y, z; };
static V3 Add(V3 a, V3 b) { return V3{ a.x + b.x, a.y + b.y, a.z + b.z }; }
static V3 Mul(V3 a, double k) { return V3{ a.x * k, a.y * k, a.z * k }; }
static double Dot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static void RotAxes(const double r[3], V3 ax[3]) {
    double P = r[0] * PI / 180, Y = r[1] * PI / 180, R = r[2] * PI / 180;
    double SP = Sin(P), CP = Cos(P), SY = Sin(Y), CY = Cos(Y), SR = Sin(R), CR = Cos(R);
    ax[0] = V3{ CP * CY, CP * SY, SP };
    ax[1] = V3{ SR * SP * CY - CR * SY, SR * SP * SY + CR * CY, -SR * CP };
    ax[2] = V3{ -(CR * SP * CY + SR * SY), CY * SR - CR * SP * SY, CR * CP };
}
static bool CompWorld(u64 comp, V3* outLoc, V3 outAx[3]) {
    u64 chain[12]; i32 n = 0; u64 c = comp;
    while (IsPtr(c) && n < 12) { chain[n++] = c; c = oAttach >= 0 ? Rd<u64>(c + oAttach) : 0; }
    if (!n) return false;
    V3 L = { 0, 0, 0 }; V3 A[3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    for (i32 i = n - 1; i >= 0; i--) {
        double rl[3], rr[3];
        if (!RPM(chain[i] + oRelLoc, rl, 24) || !RPM(chain[i] + oRelRot, rr, 24)) return false;
        L = Add(L, Add(Add(Mul(A[0], rl[0]), Mul(A[1], rl[1])), Mul(A[2], rl[2])));
        V3 a[3]; RotAxes(rr, a);
        V3 B[3];
        for (i32 k = 0; k < 3; k++) B[k] = Add(Add(Mul(A[0], a[k].x), Mul(A[1], a[k].y)), Mul(A[2], a[k].z));
        A[0] = B[0]; A[1] = B[1]; A[2] = B[2];
    }
    *outLoc = L; outAx[0] = A[0]; outAx[1] = A[1]; outAx[2] = A[2];
    return true;
}

struct Tele {
    double pitch, alt, gas, amps, mahUsed, vTot, vCell, txg, txgTarget, txgTimer, rxgTimer, secAcc, idleA, maxA, ir, startZ, thrMin, thrMax, batMax, noise;
    i32 secs, rxg; bool haveZ;
    double ppW, sp;
    bool hasCam, haveGround; V3 camLoc, camAx[3]; double fov, dist, groundZ, dg; u64 lastLog;
} T;
static u32 g_rng = 0x1234567;
static u32 Rnd() { g_rng = g_rng * 1664525u + 1013904223u; return g_rng >> 8; }
static double Rf() { return (Rnd() & 0xFFFF) / 65535.0; }

static void ResetFlight() {
    T.secs = C.startTimer; T.secAcc = 0; T.mahUsed = C.mahStart; T.haveZ = false; T.gas = 0;
    T.idleA = C.ampsIdleMin + Rf() * (C.ampsIdleMax - C.ampsIdleMin);
    T.maxA = C.ampsMaxMin + Rf() * (C.ampsMaxMax - C.ampsMaxMin);
    T.ir = 0.030 + Rf() * 0.02;
    T.rxg = -6; T.rxgTimer = 6 + Rf() * 10; T.txg = 1.0; T.txgTarget = 1.0; T.txgTimer = 4;
    T.thrMin = 1e9; T.thrMax = -1e9; T.batMax = 0; T.haveGround = false; T.hasCam = false; T.dist = -1;
}

static void (__stdcall* pXInputGetState)(u32, XINPUT_STATE*) = 0;
static double g_kbThr = 0;

static void UpdateTele(double dt, u64 pawn) {
    T.secAcc += dt; while (T.secAcc >= 1.0) { T.secAcc -= 1.0; T.secs++; }
    T.rxgTimer -= dt; if (T.rxgTimer <= 0) { T.rxg = (Rnd() & 1) ? -6 : -7; T.rxgTimer = 6 + Rf() * 14; }
    T.txgTimer -= dt; if (T.txgTimer <= 0) { T.txgTarget = 0.5 + (Rnd() % 6) * 0.1; T.txgTimer = 5 + Rf() * 10; }
    double d = T.txgTarget - T.txg; if (d > 0.01) T.txg += 0.08 * dt; else if (d < -0.01) T.txg -= 0.08 * dt;

    bool real = g_reflReady && IsPtr(pawn);
    if (real) {
        u64 root = Rd<u64>(pawn + oRoot);
        if (IsPtr(root)) {
            double loc[3], rot[3];
            if (RPM(root + oRelLoc, loc, 24) && RPM(root + oRelRot, rot, 24)) {
                if (!T.haveZ) { T.startZ = loc[2]; T.haveZ = true; }
                T.alt = (C.altRelative ? loc[2] - T.startZ : loc[2] - (g_landOk ? g_landZ : 0.0)) / 100.0;
                double p = rot[0]; while (p > 180) p -= 360; while (p < -180) p += 360;
                T.pitch = C.invertPitch ? -p : p;
                if (g_pi.dgOff >= 0) {
                    float dg = Rd<float>(pawn + g_pi.dgOff); T.dg = dg;
                    if (dg > 1.0f && dg < 100000.0f) { T.groundZ = loc[2] - dg; T.haveGround = true; }
                }
                if (!T.haveGround) { T.groundZ = T.startZ - 50.0; }
            }
        }
        u64 cam = g_pi.camOff >= 0 ? Rd<u64>(pawn + g_pi.camOff) : 0;
        if (!IsPtr(cam)) cam = root;
        T.hasCam = CompWorld(cam, &T.camLoc, T.camAx);
        T.fov = 90;
        if (oFov >= 0 && cam != root) { float f = Rd<float>(cam + oFov); if (f > 10 && f < 170) T.fov = f; }
        T.dist = -1;
        if (T.hasCam) {
            double fz = T.camAx[0].z;
            if (fz < -0.01) { double t = (T.groundZ - T.camLoc.z) / fz; T.dist = t > 0 ? t / 100.0 : 0.0; }
        }
        u64 now = GetTickCount64();
        if (now - T.lastLog > 3000) {
            T.lastLog = now;
            static char B_[512]; SB sb{ B_, 0, 512 };
            sb.s("tele: camZ="); sb.f(T.camLoc.z, 0); sb.s(" groundZ="); sb.f(T.groundZ, 0); sb.s(T.haveGround ? "(trace)" : "(spawn)"); sb.s(" dg="); sb.f(T.dg, 1);
            sb.s(" fwdZ="); sb.f(T.camAx[0].z, 3); sb.s(" dist="); sb.f(T.dist, 2); sb.s(" fov="); sb.f(T.fov, 0); sb.s(" alt="); sb.f(T.alt, 1); sb.s(" gas="); sb.f(T.gas, 0); sb.s(" ppW="); sb.f(g_ppWv, 2); sb.s(" sp="); sb.f(g_cvSP ? Rd<float>(g_cvSP + g_cvSPoff) : -1, 1);
            sb.z(); Log(B_);
        }
    }
    double thr = -1;
    if (real && g_pi.thrOff >= 0) {
        double v = g_pi.thrDouble ? Rd<double>(pawn + g_pi.thrOff) : (double)Rd<float>(pawn + g_pi.thrOff);
        if (v == v) {
            if (v < T.thrMin) T.thrMin = v; if (v > T.thrMax) T.thrMax = v;
            if (T.thrMin < -0.01) thr = (v + 1.0) * 50.0;
            else if (T.thrMax <= 1.001) thr = v * 100.0;
            else thr = v;
        }
    }
    if (thr < 0) {
        double in = 0;
        if (pXInputGetState) { XINPUT_STATE xs; memset(&xs, 0, sizeof(xs)); pXInputGetState(0, &xs); if (xs.ly > 7849) in = (xs.ly - 7849) / (32767.0 - 7849); if (xs.rt > 30 && xs.rt / 255.0 > in) in = xs.rt / 255.0; }
        if (C.keyThrUp && (GetAsyncKeyState(C.keyThrUp) & 0x8000)) g_kbThr += dt * 0.8;
        if (C.keyThrDown && (GetAsyncKeyState(C.keyThrDown) & 0x8000)) g_kbThr -= dt * 0.8;
        if (g_kbThr < 0) g_kbThr = 0; if (g_kbThr > 1) g_kbThr = 1;
        if (g_kbThr > in) in = g_kbThr;
        thr = in * 100.0;
    }
    if (thr < 0) thr = 0; if (thr > 100) thr = 100;
    T.gas += (thr - T.gas) * (dt * 12.0 > 1 ? 1 : dt * 12.0);

    double tn = T.gas / 100.0;
    T.noise += ((Rf() - 0.5) * 0.6 - T.noise) * 0.1;
    T.amps = T.idleA + tn * tn * (T.maxA - T.idleA) + T.noise; if (T.amps < 0.1) T.amps = 0.1;
    T.mahUsed += T.amps * dt / 3.6;

    double frac;
    if (real && g_pi.batOff >= 0) {
        double v = g_pi.batDouble ? Rd<double>(pawn + g_pi.batOff) : (double)Rd<float>(pawn + g_pi.batOff);
        if (v > T.batMax) T.batMax = v;
        frac = T.batMax > 0 ? v / T.batMax : 1;
    } else frac = 1.0 - (T.mahUsed - C.mahStart) / (C.packMah > 1 ? C.packMah : 1);
    if (frac < 0) frac = 0; if (frac > 1) frac = 1;
    double cellRest = 3.50 + 0.70 * frac;
    T.vCell = cellRest - T.amps * T.ir / C.cells * 3.0;
    if (T.vCell < 3.0) T.vCell = 3.0;
    T.vTot = T.vCell * C.cells;
}

static void Rect(void* br, i32 x, i32 y, i32 w, i32 h) { GdipFillRectangleI(g_gpG, br, x, y, w, h); }
static void Outlined(void* br, i32 x, i32 y, i32 w, i32 h) { Rect(g_brB, x - 1, y - 1, w + 2, h + 2); Rect(br, x, y, w, h); }
static bool Project(V3 d, i32 W, i32 H, double* cx, double* cy) {
    double fwd = Dot(d, T.camAx[0]); if (fwd < 0.05) return false;
    double th = Sin(T.fov * PI / 360) / Cos(T.fov * PI / 360); if (th < 0.01) return false;
    double f = (W * 0.5) / th;
    double x = W * 0.5 + f * Dot(d, T.camAx[1]) / fwd, y = H * 0.5 - f * Dot(d, T.camAx[2]) / fwd;
    *cx = x * CW / W; *cy = y * CH / H; return true;
}
static i32 g_hz[4];
static bool HorizonLine(i32 W, i32 H, double* x0, double* y0, double* ux, double* uy) {
    if (!T.hasCam) return false;
    V3 F = T.camAx[0]; double hl = Sqrt(F.x * F.x + F.y * F.y); if (hl < 0.05) return false;
    V3 h = { F.x / hl, F.y / hl, 0 }, hr = { -h.y, h.x, 0 };
    double ax, ay, bx, by, cx, cy;
    if (!Project(h, W, H, &cx, &cy) || !Project(Add(h, Mul(hr, -0.7)), W, H, &ax, &ay) || !Project(Add(h, Mul(hr, 0.7)), W, H, &bx, &by)) return false;
    double dx = bx - ax, dy = by - ay, l = Sqrt(dx * dx + dy * dy); if (l < 1) return false;
    *x0 = cx; *y0 = cy; *ux = dx / l; *uy = dy / l; return true;
}
static void DrawAimAndHorizon(i32 W, i32 H) {
    double x0, y0, ux, uy;
    if (HorizonLine(W, H, &x0, &y0, &ux, &uy)) {
        const double sp = 20.0, gap = 95.0, span = 330.0;
        for (double t = -span; t <= span; t += sp) {
            if (t > -gap && t < gap) continue;
            i32 px = (i32)(x0 + ux * t), py = (i32)(y0 + uy * t);
            if (px < 4 || py < 4 || px > CW - 6 || py > CH - 6) continue;
            Outlined(g_brDot, px - 1, py - 1, 3, 3);
        }
    }
    const float cx = CW * 0.5f, cy = CH * 0.5f, arm = 12.0f;
    GdipDrawLine(g_gpG, g_penB, cx - arm, cy, cx + arm, cy); GdipDrawLine(g_gpG, g_penB, cx, cy - arm, cx, cy + arm);
    GdipDrawLine(g_gpG, g_penW, cx - arm, cy, cx + arm, cy); GdipDrawLine(g_gpG, g_penW, cx, cy - arm, cx, cy + arm);
}

struct CapFrame { HDC dc; HANDLE bmp; u32* bits; i32 w, h; u64 t; bool ok; };
static CapFrame g_ring[8]; static volatile i32 g_ringIdx = 0;
static volatile i32 g_capOn = 0, g_capBusy = 0, g_frozen = 0, g_capSlot = -1;
static volatile i32 g_capX = 0, g_capY = 0, g_capW = 0, g_capH = 0;
static volatile u64 g_manualKeyMs = 0;
static u64 NowMs() { i64 c, f; QueryPerformanceCounter(&c); QueryPerformanceFrequency(&f); return (u64)(c / (f / 1000)); }
static void EnsureCap(CapFrame& f, i32 w, i32 h) {
    if (f.ok && f.w == w && f.h == h) return;
    if (f.dc) DeleteDC(f.dc); if (f.bmp) DeleteObject(f.bmp); f.dc = 0; f.bmp = 0;
    HDC scr = GetDC(0); f.dc = CreateCompatibleDC(scr); ReleaseDC(0, scr);
    BITMAPINFO bi; memset(&bi, 0, sizeof(bi)); bi.h.size = sizeof(BITMAPINFOHEADER); bi.h.w = w; bi.h.h = -h; bi.h.planes = 1; bi.h.bits = 32;
    void* bits = 0; f.bmp = CreateDIBSection(f.dc, &bi, 0, &bits, 0, 0); f.bits = (u32*)bits; SelectObject(f.dc, f.bmp);
    f.w = w; f.h = h; f.ok = f.bits != 0; f.t = 0;
}
static u32 __stdcall CaptureThread(void*) {
    u64 lastLog = 0; u64 sum = 0; i32 cnt = 0;
    for (;;) {
        if (!g_capOn || g_frozen || g_capW < 100) { Sleep(10); continue; }
        g_capBusy = 1;
        if (g_frozen) { g_capBusy = 0; continue; }
        i32 i = (g_ringIdx + 1) & 7;
        g_capSlot = i;
        CapFrame& f = g_ring[i];
        EnsureCap(f, g_capW, g_capH);
        u64 t0 = NowMs();
        HDC scr = GetDC(0);
        if (f.ok && BitBlt(f.dc, 0, 0, f.w, f.h, scr, g_capX, g_capY, 0x00CC0020)) { f.t = t0; g_ringIdx = i; }
        ReleaseDC(0, scr);
        g_capSlot = -1; g_capBusy = 0;
        u64 t1 = NowMs(); sum += t1 - t0; cnt++;
        if (t1 - lastLog > 30000 && cnt) { lastLog = t1; static char B_[96]; SB sb{ B_, 0, 96 }; sb.s("capture: avg ms="); sb.f((double)sum / cnt, 1); sb.z(); Log(B_); sum = 0; cnt = 0; }
        u64 el = NowMs() - t0; if (el < 16) Sleep((u32)(16 - el));
    }
    return 0;
}
static u32* g_frzBuf = 0; static i32 g_frzW = 0, g_frzH = 0;
static bool GrabFreezeFrame(u64 trigMs, u64 pickMs, i32 w, i32 h) {
    g_frozen = 1;
    i32 busy = g_capSlot;
    i32 best = -1; u64 bestT = 0; i32 newest = -1; u64 newT = 0;
    for (i32 i = 0; i < 8; i++) {
        CapFrame& f = g_ring[i]; if (i == busy || !f.ok || !f.t || f.w != w || f.h != h) continue;
        if (f.t > newT) { newT = f.t; newest = i; }
        if (f.t + (u64)C.frzBack <= pickMs && f.t > bestT) { bestT = f.t; best = i; }
    }
    if (best < 0) { u64 oT = ~0ull; for (i32 i = 0; i < 8; i++) { CapFrame& f = g_ring[i]; if (i != busy && f.ok && f.t && f.w == w && f.h == h && f.t < oT) { oT = f.t; best = i; } } }
    if (best < 0) best = newest;
    if (best < 0) return false;
    if (!g_frzBuf || g_frzW != w || g_frzH != h) { g_frzBuf = (u32*)VirtualAlloc(0, (u64)w * h * 4, 0x3000, 4); g_frzW = w; g_frzH = h; }
    if (!g_frzBuf) return false;
    memcpy(g_frzBuf, g_ring[best].bits, (u64)w * h * 4);
    static char B_[96]; SB sb{ B_, 0, 96 }; sb.s("impact: freeze frame age ms="); sb.i((i64)(trigMs - g_ring[best].t)); sb.s(" before-impact ms="); sb.i((i64)pickMs - (i64)g_ring[best].t); sb.z(); Log(B_);
    return true;
}
static void* g_fontNI = 0; static void* g_brNI = 0; static void* g_brNIsh = 0; static void* g_fmtC = 0; static i32 g_fontNIh = 0;
static void ComposeImpact(i32 w, i32 h, bool phase2) {
    EnsureDib(w, h);
    for (i32 y = 0; y < h; y++) {
        const u32* src = (const u32*)(g_canvas + (u64)((i64)y * CH / h) * CW * 4);
        const u32* fr = g_frzBuf + (u64)y * w;
        u32* dst = g_dibBits + (u64)y * w;
        for (i32 x = 0; x < w; x++) {
            u32 o = src[g_xmap[x]], b = fr[x];
            u32 oa = o >> 24, ia = 255 - oa;
            u32 r = (o >> 16 & 255) + ((b >> 16 & 255) * ia) / 255, g = (o >> 8 & 255) + ((b >> 8 & 255) * ia) / 255, bl = (o & 255) + ((b & 255) * ia) / 255;
            if (phase2) { u32 ga = (u32)(C.grayA * 255), gi = 255 - ga; r = (r * gi + 128 * ga) / 255; g = (g * gi + 128 * ga) / 255; bl = (bl * gi + 128 * ga) / 255; }
            if (r > 255) r = 255; if (g > 255) g = 255; if (bl > 255) bl = 255;
            dst[x] = 0xFF000000u | (r << 16) | (g << 8) | bl;
        }
    }
    if (!phase2) return;
    i32 fh = (i32)(h * 0.075); if (fh < 12) fh = 12;
    if (!g_fontNI || g_fontNIh != fh) {
        void* fam = 0; if (GdipCreateFontFamilyFromName(L"Arial", 0, &fam) != 0) GdipGetGenericFontFamilySerif(&fam);
        GdipCreateFont(fam, (float)fh, 0, 2, &g_fontNI); g_fontNIh = fh;
        if (!g_brNI) { GdipCreateSolidFill(0xFFF2F2F2, &g_brNI); GdipCreateSolidFill(0x30000000, &g_brNIsh); GdipCreateStringFormat(0x1000 | 0x4000, 0, &g_fmtC); GdipSetStringFormatAlign(g_fmtC, 1); }
    }
    void* bmp = 0; void* g = 0;
    if (GdipCreateBitmapFromScan0(w, h, w * 4, 0x000E200B, (u8*)g_dibBits, &bmp) != 0) return;
    if (GdipGetImageGraphicsContext(bmp, &g) != 0) return;
    GdipSetTextRenderingHint(g, 4); GdipSetSmoothingMode(g, 4);
    const wc* l1 = L"Visible"; const wc* l2 = L"NO IMAGE";
    float y1 = h * 0.302f, y2 = h * 0.390f;
    for (i32 dy = -3; dy <= 4; dy++) for (i32 dx = -3; dx <= 4; dx++) {
        if (dx * dx + dy * dy > 18) continue;
        RectF a = { (float)dx, y1 + dy, (float)w, (float)fh * 1.5f }, b = { (float)dx, y2 + dy, (float)w, (float)fh * 1.5f };
        GdipDrawString(g, l1, 7, g_fontNI, &a, g_fmtC, g_brNIsh); GdipDrawString(g, l2, 8, g_fontNI, &b, g_fmtC, g_brNIsh);
    }
    RectF a = { 0, y1, (float)w, (float)fh * 1.5f }, b = { 0, y2, (float)w, (float)fh * 1.5f };
    GdipDrawString(g, l1, 7, g_fontNI, &a, g_fmtC, g_brNI); GdipDrawString(g, l2, 8, g_fontNI, &b, g_fmtC, g_brNI);
    GdipDeleteGraphics(g); GdipDisposeImage(bmp);
}

static wc g_lastKey[512];
static bool Render(i32 w, i32 h) {
    wc s[12][32];
    i32 mm = T.secs / 60, ss = T.secs % 60;
    if (mm > 99) mm = 99;
    s[0][0] = '0' + (mm / 10) % 10; s[0][1] = '0' + mm % 10; s[0][2] = ':'; s[0][3] = '0' + ss / 10; s[0][4] = '0' + ss % 10; s[0][5] = 0;
    FmtW(s[1], T.alt, 1, true, 0);
    FmtW(s[2], T.vTot, 1, false, L"V");
    FmtW(s[3], T.vCell, 2, false, L"V");
    FmtW(s[4], T.pitch, 1, false, 0);
    FmtW(s[5], T.amps, 1, false, 0);
    FmtW(s[6], (double)(i64)T.mahUsed, 0, false, 0);
    FmtW(s[7], (double)T.rxg, 0, false, 0);
    FmtW(s[8], (double)(i64)(T.gas + 0.5), 0, false, 0);
    FmtW(s[9], T.txg, 1, false, 0);
    s[10][0] = 'F'; s[10][1] = 'P'; s[10][2] = 'S'; s[10][3] = ':'; s[10][4] = ' '; FmtW(s[10] + 5, (double)g_fpsShown, 0, false, 0);
    wc key[512]; key[0] = 0;
    for (i32 i = 0; i < 11; i++) { wcat(key, s[i], 512); wcat(key, L"|", 512); }
    {
        double x0 = 0, y0 = 0, ux = 0, uy = 0; bool hz = HorizonLine(w, h, &x0, &y0, &ux, &uy);
        wc t[64]; i32 n = 0; i32 v[5] = { 0, hz ? (i32)x0 : -9999, hz ? (i32)y0 : -9999, hz ? (i32)(ux * 1000) : 0, hz ? (i32)(uy * 1000) : 0 };
        for (i32 k = 0; k < 5; k++) { n += FmtW(t + n, (double)v[k], 0, false, L"|"); }
        wcat(key, t, 512);
    }
    bool sizeChanged = (w != g_dibW || h != g_dibH);
    bool same = !sizeChanged; for (i32 i = 0; same && (key[i] || g_lastKey[i]); i++) if (key[i] != g_lastKey[i]) same = false;
    if (same) return false;
    wcpy(g_lastKey, key, 512);
    GdipGraphicsClear(g_gpG, 0x00000000);
    if (C.useTemplate) GdipDrawImageRectI(g_gpG, g_gpTpl, 0, 0, CW, CH); else DrawStaticLabels();
    Text(s[0], 16, 42, false);
    Text(s[1], 86, 81, true);
    Text(s[2], 430, 18, false);
    Text(s[3], 565, 18, false);
    Text(s[4], 686, 41, false);
    Text(s[5], 884, 326, true);
    Text(s[6], 911, 356, true);
    Text(s[7], 72, 409, false);
    Text(s[8], 924, 409, true);
    Text(s[9], 68, 444, false);
    Text(s[10], 447, 531, false);
    DrawAimAndHorizon(w, h);
    EnsureDib(w, h);
    Upscale();
    return true;
}

static i64 __stdcall WndProc(HWND h, u32 m, u64 wp, i64 lp) {
    if (m == 2) { PostQuitMessage(0); return 0; }
    return DefWindowProcW(h, m, wp, lp);
}

extern "C" void Entry() {
    InitializeCriticalSection(&g_logCs); InitializeCriticalSection(&g_nameCs);
    GetModuleFileNameW(0, g_dir, 520);
    { i32 n = wlen(g_dir); while (n > 0 && g_dir[n - 1] != '\\') n--; g_dir[n] = 0; }
    wcpy(g_logPath, g_dir, 600); wcat(g_logPath, L"osd_log.txt", 600);
    wcpy(g_dumpPath, g_dir, 600); wcat(g_dumpPath, L"osd_dump.txt", 600);
    wcpy(g_cfgPath, g_dir, 600); wcat(g_cfgPath, L"config.ini", 600);
    CreateMutexW(0, 1, L"FPV_OSD_Overlay_SingleInstance");
    if (GetLastError() == 183) ExitProcess(0);
    TruncFile(g_logPath); TruncFile(g_dumpPath);
    Log("FPV OSD overlay v2 start");
    LoadConfig();
    LoadCVarConfig();
    {
        void* u = GetModuleHandleW(L"user32.dll");
        typedef i32 (__stdcall* DpiCtx)(void*);
        DpiCtx f = u ? (DpiCtx)GetProcAddress(u, "SetProcessDpiAwarenessContext") : 0;
        if (f) f((void*)(i64)-4);
        else { typedef i32 (__stdcall* Dpi)(); Dpi g = u ? (Dpi)GetProcAddress(u, "SetProcessDPIAware") : 0; if (g) g(); }
        void* xi = LoadLibraryW(L"xinput1_4.dll"); if (!xi) xi = LoadLibraryW(L"xinput9_1_0.dll");
        if (xi) pXInputGetState = (void (__stdcall*)(u32, XINPUT_STATE*))GetProcAddress(xi, "XInputGetState");
    }
    if (!InitRender()) { MessageBoxW(0, L"OSD init failed (osd_template.png / GDI+). See osd_log.txt", L"FPV OSD", 0x10); ExitProcess(1); }
    { i64 t; QueryPerformanceCounter(&t); g_rng ^= (u32)t; }
    ResetFlight();

    WNDCLASSEXW wcx; memset(&wcx, 0, sizeof(wcx)); wcx.cbSize = sizeof(wcx); wcx.proc = WndProc; wcx.inst = GetModuleHandleW(0); wcx.cls = L"FPV_OSD_Overlay_v2";
    RegisterClassExW(&wcx);
    HWND ov = CreateWindowExW(0x80000 | 0x20 | 0x8 | 0x80 | 0x08000000, L"FPV_OSD_Overlay_v2", L"FPV OSD", 0x80000000, 0, 0, CW, CH, 0, 0, wcx.inst, 0);
    { void* u = GetModuleHandleW(L"user32.dll"); typedef i32 (__stdcall* SWDA)(HWND, u32); SWDA f = u ? (SWDA)GetProcAddress(u, "SetWindowDisplayAffinity") : 0; if (f) { if (!f(ov, 0x11)) Log("overlay: exclude-from-capture not supported"); } }
    g_wakeEvt = CreateEventW(0, 0, 0, 0); CreateThread(0, 0, PawnWatch, 0, 0, 0);
    if (!ov) ExitProcess(1);
    RegisterHotKey(ov, 1, 0x4000, C.keyToggle); RegisterHotKey(ov, 2, 0x4000, C.keyReset); RegisterHotKey(ov, 3, 0x4000, C.keyDump); RegisterHotKey(ov, 4, 0x4000, 0x77);
    CreateThread(0, 0, Worker, 0, 0, 0);
    CreateThread(0, 0, CaptureThread, 0, 0, 0);

    i64 freq, last; QueryPerformanceFrequency(&freq); QueryPerformanceCounter(&last);
    HWND game = 0; bool everSeen = false, shown = false; u64 lastFind = 0, lastTop = 0; POINT lastPos = { -99999, -99999 };
    u64 lastCls = 0; u64 lastPawn = 0; bool wasK = false; i32 lastLink = -1;
    MSG msg;
    for (;;) {
        while (PeekMessageW(&msg, 0, 0, 0, 1)) {
            if (msg.message == 0x12) ExitProcess(0);
            if (msg.message == 0x312) {
                if (msg.wParam == 1) { g_userHidden = !g_userHidden; Log(g_userHidden ? "toggle: OSD off" : "toggle: OSD on"); }
                else if (msg.wParam == 2) ResetFlight();
                else if (msg.wParam == 3) g_dumpReq = 1;
                else if (msg.wParam == 4) { g_gfxOff = !g_gfxOff; Log(g_gfxOff ? "graphics tweaks: OFF (game defaults)" : "graphics tweaks: ON"); }
            }
            TranslateMessage(&msg); DispatchMessageW(&msg);
        }
        i64 now; QueryPerformanceCounter(&now);
        double dt = (double)(now - last) / freq; last = now; if (dt > 0.1) dt = 0.1;
        u64 tick = GetTickCount64();

        if (g_proc) { u32 ec = 0; if (GetExitCodeProcess(g_proc, &ec) && ec != 259) { Log("game exited"); ExitProcess(0); } }
        if (!g_proc) {
            if (tick - lastFind > 1000) { lastFind = tick; u32 pid = FindGamePid(); if (pid) AttachProcess(pid); }
            if (!g_proc) { if (shown) { ShowWindow(ov, 0); shown = false; } Sleep(100); continue; }
        }
        if (!game || !IsWindow(game)) {
            if (tick - lastFind > 500) { lastFind = tick; g_targetPid = g_pid; game = 0; EnumWindows(EnumProc, (i64)&game); if (game) everSeen = true; }
        }
        if (!game) { if (shown) { ShowWindow(ov, 0); shown = false; } Sleep(50); continue; }

        if (g_kamikazeActive) {
            const i32 mk[] = { 'R', 'Q', 0x1B, 0x08, 0x09, 'M' };
            for (i32 k = 0; k < 6; k++) if (GetAsyncKeyState(mk[k]) & 0x8000) g_manualKeyMs = NowMs();
        }
        u64 pawn = 0;
        static i32 frzState = 0; static u64 frzStart = 0; static bool prevBroken = false; static double lastSpd = 0, fastSpd = 0; static u64 lastMoveT = 0, lastFastT = 0; static double lpx = 0, lpy = 0, lpz = 0; static bool lpOk = false;
        bool trigger = false; u64 trigMs = NowMs(); u64 pickMs = 0; static bool frzSoft = false; static u64 frzPawn = 0; bool softTrig = false;
        if (g_reflReady) {
            pawn = ReadPawn();
            if (C.impactFx && frzState == 0) {
                bool wasKam = lastPawn && g_pi.kamikaze;
                if (wasKam && pawn == lastPawn) {
                    u64 root = Rd<u64>(pawn + oRoot); double lc[3];
                    if (IsPtr(root) && RPM(root + oRelLoc, lc, 24)) {
                        if (!lpOk) { lpx = lc[0]; lpy = lc[1]; lpz = lc[2]; lpOk = true; lastMoveT = trigMs; }
                        else {
                            double dx = lc[0] - lpx, dy = lc[1] - lpy, dz = lc[2] - lpz; double d2 = dx * dx + dy * dy + dz * dz;
                            u64 el = trigMs - lastMoveT;
                            if (d2 > 0.01 || el > 90) {
                                double spd = Sqrt(d2) / 100.0 / ((el ? el : 1) / 1000.0);
                                if (fastSpd > 8.0 && spd < fastSpd * 0.35) { trigger = true; softTrig = true; pickMs = lastFastT; static char B_[96]; SB sb{ B_, 0, 96 }; sb.s("impact: sudden stop "); sb.f(fastSpd, 1); sb.s(" -> "); sb.f(spd, 1); sb.s(" m/s"); sb.z(); Log(B_); fastSpd = 0; }
                                else {
                                    if (spd >= fastSpd * 0.7 || spd > 3.0) { lastFastT = lastMoveT; }
                                    fastSpd = fastSpd <= 0 ? spd : fastSpd * 0.6 + spd * 0.4;
                                }
                                lastSpd = spd; lpx = lc[0]; lpy = lc[1]; lpz = lc[2]; lastMoveT = trigMs;
                            }
                        }
                    }
                    if (!trigger && g_pi.dgOff >= 0 && IsPtr(root)) {
                        static double pz = 0; static u64 pzT = 0; static u64 pzPawn = 0; static double vz = 0; static u64 loopMs = 30; static u64 lastIt = 0;
                        if (lastIt) { u64 e = trigMs - lastIt; loopMs = (loopMs * 3 + (e > 200 ? 200 : e)) / 4; } lastIt = trigMs;
                        double z = Rd<double>(root + oRelLoc + 16);
                        if (pzPawn != pawn) { pzPawn = pawn; pz = z; pzT = trigMs; vz = 0; }
                        else if (z != pz && trigMs > pzT) { vz = (z - pz) / ((trigMs - pzT) / 1000.0); pz = z; pzT = trigMs; }
                        float dg = Rd<float>(pawn + g_pi.dgOff);
                        double lead = (loopMs * 1.5 + 17.0) / 1000.0; if (lead > 0.12) lead = 0.12;
                        if (vz < -300.0 && dg > 1.0f && dg < 600.0f && dg / -vz < lead) {
                            trigger = true; softTrig = true; pickMs = trigMs;
                            static char B_[128]; SB sb{ B_, 0, 128 }; sb.s("impact: ground ahead dg="); sb.f(dg, 0); sb.s("cm vz="); sb.f(vz / 100.0, 1); sb.s("m/s lead ms="); sb.i((i64)(lead * 1000)); sb.z(); Log(B_);
                        }
                    }
                    bool br = g_pi.brokenOff >= 0 && (Rd<u8>(pawn + g_pi.brokenOff) & g_pi.brokenMask);
                    if (br && !prevBroken && !trigger) { trigger = true; pickMs = lastFastT ? lastFastT : trigMs - 40; Log("impact: drone broken"); }
                    prevBroken = br;
                } else if (wasKam && pawn != lastPawn && !prevBroken) {
                    bool manual = trigMs - g_manualKeyMs < 800;
                    if (!manual && lastSpd >= C.frzSpeed) { trigger = true; pickMs = lastFastT ? lastFastT : trigMs - 60; Log("impact: drone lost"); }
                    else Log(manual ? "impact: drone left manually, no freeze" : "impact: drone lost while hovering, no freeze");
                }
                if (pawn != lastPawn) { prevBroken = false; lpOk = false; lastSpd = 0; fastSpd = 0; lastFastT = 0; }
            }
            if (g_link != lastLink) { lastLink = g_link; const char* st[] = { "no GWorld", "world ok, no player controller", "controller ok, no pawn", "pawn ok" }; Log("link: ", st[g_link]); }
            if (pawn != lastPawn) {
                lastPawn = pawn;
                u64 cls = pawn ? ObjClass(pawn) : 0;
                if (cls && cls != lastCls) { lastCls = cls; AnalyzePawnClass(cls); }
                if (!cls) { lastCls = 0; memset(&g_pi, 0, sizeof(g_pi)); }
                g_uiOff = (pawn && g_pi.kamikaze) ? g_pi.uiOff : -1;
                g_camOff = (pawn && g_pi.kamikaze) ? g_pi.camOff : -1;
                g_ppOff = (pawn && g_pi.kamikaze) ? g_pi.ppOff : -1;
                g_pawn = pawn; g_pawnSerial = g_pawnSerial + 1;
                bool k = pawn && g_pi.kamikaze;
                if (k) ResetFlight();
                if (k != wasK) { wasK = k; }
            }
        }
        g_watchPawn = (g_reflReady && pawn && g_pi.kamikaze && frzState == 0) ? pawn : 0; g_watchOn = g_watchPawn ? 1 : 0;
        bool kam = g_reflReady ? (pawn && g_pi.kamikaze) : (C.fallbackShow != 0);
        g_kamikazeActive = (g_reflReady && pawn && g_pi.kamikaze) ? 1 : 0;
        if (g_kamikazeActive) FixCamera(pawn);

        {
            RECT rc0; GetClientRect(game, &rc0); POINT p0 = { 0, 0 }; ClientToScreen(game, &p0);
            g_capX = p0.x; g_capY = p0.y; g_capW = rc0.r; g_capH = rc0.b;
            g_capOn = (C.impactFx && g_kamikazeActive && shown && frzState == 0) ? 1 : 0;
            if (trigger && shown && rc0.r >= 100 && rc0.b >= 100) {
                if (GrabFreezeFrame(trigMs, pickMs ? pickMs : trigMs, rc0.r, rc0.b)) { frzState = 1; frzStart = trigMs; frzSoft = softTrig; frzPawn = pawn; ComposeImpact(rc0.r, rc0.b, false); }
                else g_frozen = 0;
                if (frzState) { HDC scr = GetDC(0); POINT src = { 0, 0 }; SIZE sz = { rc0.r, rc0.b }; BLENDFUNCTION bf = { 0, 0, 255, 1 }; UpdateLayeredWindow(ov, scr, &p0, &sz, g_memDC, &src, 0, &bf, 2); ReleaseDC(0, scr); }
            }
            if (frzState) {
                u64 el = NowMs() - frzStart;
                if (frzState == 1 && frzSoft && el >= 300) {
                    frzSoft = false;
                    u64 pn = ReadPawn();
                    bool br = pn && pn == frzPawn && g_pi.brokenOff >= 0 && (Rd<u8>(pn + g_pi.brokenOff) & g_pi.brokenMask);
                    if (pn && pn == frzPawn && g_pi.kamikaze && !br) { Log("impact: drone survived, freeze cancelled"); frzState = 0; g_frozen = 0; g_lastKey[0] = 0; ShowWindow(ov, 0); shown = false; continue; }
                }
                if (frzState == 1 && el >= (u64)(C.frzTime * 1000)) {
                    frzState = 2; ComposeImpact(g_frzW, g_frzH, true);
                    HDC scr = GetDC(0); POINT src = { 0, 0 }; SIZE sz = { g_frzW, g_frzH }; BLENDFUNCTION bf = { 0, 0, 255, 1 }; UpdateLayeredWindow(ov, scr, &p0, &sz, g_memDC, &src, 0, &bf, 2); ReleaseDC(0, scr);
                }
                if (el >= (u64)((C.frzTime + C.noImgTime) * 1000) || g_userHidden) { frzState = 0; g_frozen = 0; g_lastKey[0] = 0; ShowWindow(ov, 0); shown = false; }
                else { if (!shown) { ShowWindow(ov, 4); shown = true; } Sleep(15); continue; }
            }
        }

        CURSORINFO ci; memset(&ci, 0, sizeof(ci)); ci.cbSize = sizeof(ci);
        bool cursor = true; if (GetCursorInfo(&ci)) cursor = (ci.flags & 1) != 0;
        bool fg = GetForegroundWindow() == game;
        bool show = kam && !cursor && fg && !g_userHidden && IsWindowVisible(game) && !IsIconic(game);

        if (!show) { if (shown) { ShowWindow(ov, 0); shown = false; } Sleep(30); continue; }

        RECT rc; GetClientRect(game, &rc); POINT pt = { 0, 0 }; ClientToScreen(game, &pt);
        i32 w = rc.r, h = rc.b; g_winH = h;
        if (w < 100 || h < 100) { Sleep(30); continue; }
        UpdateTele(dt, pawn);
        bool changed = Render(w, h);
        if (changed || pt.x != lastPos.x || pt.y != lastPos.y || !shown) {
            HDC scr = GetDC(0); POINT src = { 0, 0 }; SIZE sz = { w, h }; BLENDFUNCTION bf = { 0, 0, 255, 1 };
            UpdateLayeredWindow(ov, scr, &pt, &sz, g_memDC, &src, 0, &bf, 2);
            ReleaseDC(0, scr); lastPos = pt;
        }
        if (!shown) { ShowWindow(ov, 4); shown = true; lastTop = 0; }
        if (tick - lastTop > 1000) { lastTop = tick; SetWindowPos(ov, (HWND)(i64)-1, 0, 0, 0, 0, 0x1 | 0x2 | 0x10); }
        WaitForSingleObject(g_wakeEvt, 15);
    }
}
