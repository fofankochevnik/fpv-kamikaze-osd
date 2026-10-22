#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "psapi.lib")

int main() {
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W pe = { sizeof(pe) };
        if (Process32FirstW(snapshot, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, L"FPVKamikazeDrone-Win64-Shipping.exe") == 0) {
                    pid = pe.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &pe));
        }
        CloseHandle(snapshot);
    }
    if (!pid) {
        printf("Game not running.\n");
        return 0;
    }

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) return 1;

    // Scan committed memory pages for "WBP_FPV_Drone"
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t addr = (uintptr_t)si.lpMinimumApplicationAddress;
    uintptr_t maxAddr = (uintptr_t)si.lpMaximumApplicationAddress;

    const char target[] = "WBP_FPV_Drone";
    const size_t targetLen = strlen(target);
    int foundCount = 0;

    std::vector<BYTE> buffer;

    while (addr < maxAddr && VirtualQueryEx(hProcess, (LPCVOID)addr, &mbi, sizeof(mbi))) {
        if ((mbi.State == MEM_COMMIT) && (mbi.Protect & (PAGE_READWRITE | PAGE_READONLY | PAGE_EXECUTE_READWRITE))) {
            buffer.resize(mbi.RegionSize);
            SIZE_T bytesRead = 0;
            if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead)) {
                for (size_t i = 0; i + targetLen < bytesRead; i++) {
                    if (memcmp(buffer.data() + i, target, targetLen) == 0) {
                        printf("Found '%s' at 0x%llX (Protect: 0x%X)\n", target, (uintptr_t)mbi.BaseAddress + i, mbi.Protect);
                        foundCount++;
                        if (foundCount > 10) break;
                    }
                }
            }
        }
        addr = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
        if (foundCount > 10) break;
    }

    printf("Total found: %d\n", foundCount);
    CloseHandle(hProcess);
    return 0;
}
