#include "Header.h"

// C++ code
//
BOOL WINAPI DllMain(HINSTANCE module_handle, DWORD reason_for_call, LPVOID reserved) {
    if (reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module_handle);
    }
    return TRUE;
}

