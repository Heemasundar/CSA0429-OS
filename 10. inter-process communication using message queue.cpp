#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define SHM_SIZE 1024
#define SHM_NAME "Local\\SharedMemoryExample"

int main() {
    HANDLE hMapFile;
    LPVOID pBuf;
    char input[SHM_SIZE];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Create shared memory
    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHM_SIZE, SHM_NAME);
    if (hMapFile == NULL) {
        printf("Could not create file mapping (%d).\n", GetLastError());
        return 1;
    }

    // Map shared memory
    pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);
    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    // Initialize child process startup info
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Create child process
    if (!CreateProcess(NULL, "shared_memory_reader.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("Could not create child process (%d).\n", GetLastError());
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }

    // Parent process writes to shared memory
    printf("Parent: Enter a message: ");
    fgets(input, SHM_SIZE, stdin);
    sprintf((char *)pBuf, "%s", input);

    printf("Parent: Waiting for child to read...\n");
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
