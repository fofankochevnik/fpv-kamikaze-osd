#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#pragma comment(lib, "psapi.lib")

const uintptr_t RVA_GWORLD = 0xa219d88;
const uintptr_t RVA_GUOBJECTARRAY = 0x9db24e0;
const uintptr_t RVA_GUOBJECTARRAY_COUNT = 0x9db2504;
const uintptr_t RVA_FNAMEPOOL = 0x9fab2c0;

int main() {
    printf("Diagnostic tool starting...\n");
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe = { sizeof(pe) };
        if (Process32FirstW(snapshot, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, L"FPVKamikazeDrone-Win64-Shipping.exe") == 0) {
                    pid = pe.th32ProcessID;
                    printf("Found Shipping PID: %d\n", pid);
                    break;
                }
            } while (Process32NextW(snapshot, &pe));
        }
        CloseHandle(snapshot);
    }

    if (!pid) {
        printf("Game (FPVKamikazeDrone-Win64-Shipping.exe) is NOT running.\n");
        return 0;
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) {
        printf("Failed to OpenProcess with ALL_ACCESS, trying READ|WRITE...\n");
        hProc = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    }
    if (!hProc) {
        printf("Failed to OpenProcess! Error: %d\n", GetLastError());
        return 1;
    }

    HMODULE hMods[1024];
    DWORD cbNeeded = 0;
    uintptr_t base = 0;
    if (EnumProcessModules(hProc, hMods, sizeof(hMods), &cbNeeded)) {
        base = (uintptr_t)hMods[0];
        printf("Base address: 0x%llX\n", base);
    } else {
        printf("EnumProcessModules failed! Error: %d\n", GetLastError());
    }

    if (base) {
        uintptr_t uworld = 0;
        ReadProcessMemory(hProc, (LPCVOID)(base + RVA_GWORLD), &uworld, sizeof(uworld), NULL);
        printf("GWorld (base + 0x%llX): 0x%llX\n", RVA_GWORLD, uworld);

        int32_t numObj = 0;
        ReadProcessMemory(hProc, (LPCVOID)(base + RVA_GUOBJECTARRAY_COUNT), &numObj, sizeof(numObj), NULL);
        printf("GUObjectArray Count: %d\n", numObj);

        uintptr_t chunkTable = 0;
        ReadProcessMemory(hProc, (LPCVOID)(base + RVA_GUOBJECTARRAY), &chunkTable, sizeof(chunkTable), NULL);
        printf("GUObjectArray ChunkTable: 0x%llX\n", chunkTable);
    }

    CloseHandle(hProc);
    return 0;
}
