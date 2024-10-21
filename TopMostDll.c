#include <windows.h>
#include <stdio.h>

#define ID_ALWAYS_ON_TOP 1 // ID for the Always on Top menu item

HINSTANCE hInst;
HMENU hSysMenu;

// Function to toggle the "Always on Top" state
void ToggleAlwaysOnTop(HWND hwnd) {
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (style & WS_EX_TOPMOST) {
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    } else {
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
}

// Window procedure hook function
LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        CWPSTRUCT *pwp = (CWPSTRUCT *)lParam;
        if (pwp->message == WM_CREATE) {
            // Add "Always on Top" to the system menu when the window is created
            hSysMenu = GetSystemMenu(pwp->hwnd, FALSE);
            if (hSysMenu) {
                AppendMenu(hSysMenu, MF_STRING, ID_ALWAYS_ON_TOP, "Always on Top");
            }
        }
        else if (pwp->message == WM_SYSCOMMAND) {
            MessageBoxA(0, "hi", "hi", 0);
          
            // Handle the "Always on Top" click
            if ((wParam & 0xFFF0) == ID_ALWAYS_ON_TOP) {
                ToggleAlwaysOnTop(pwp->hwnd);
                return 0; // Return 0 to indicate message was processed
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HHOOK hHook = NULL;

// DLL entry point
BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        hInst = hInstance;
    }
    return TRUE;
}

// Install the hook
__declspec(dllexport) void InstallHook() {
    hHook = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, hInst, 0);
    if (hHook) {
        MessageBox(NULL, "Hook installed!", "Info", MB_OK);
    } else {
        MessageBox(NULL, "Failed to install hook!", "Error", MB_OK);
    }
}

// Uninstall the hook
__declspec(dllexport) void UninstallHook() {
    if (hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
        MessageBox(NULL, "Hook uninstalled!", "Info", MB_OK);
    }
}

