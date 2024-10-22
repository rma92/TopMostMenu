#include <windows.h>
#include <stdio.h>

#define ID_ALWAYS_ON_TOP 0x1000 // ID for the Always on Top menu item

HINSTANCE hInst;
HMENU hSysMenu;

// Function to toggle the "Always on Top" state
void ToggleAlwaysOnTop(HWND hwnd) {
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    LPCSTR buf = "Chicken!";
    SetWindowTextA(hwnd, buf);
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
    if (nCode == HCBT_SYSCOMMAND)
    {
      char buf[255];
      wsprintfA(buf, "wParam: 0x%X lParam: 0x%X\r\n", wParam, lParam);
      OutputDebugString( buf );
      if( wParam == SC_MOVE )
      {
        
      }
      else if( wParam == ID_ALWAYS_ON_TOP )
      {
        //MessageBoxA(0, "A", "Move", 0);
        HWND hwnd = GetForegroundWindow();
        ToggleAlwaysOnTop(hwnd);
        return 0;
      }
    }
    else if (nCode == HC_ACTION) {
        CWPSTRUCT *pwp = (CWPSTRUCT *)lParam;
        if (pwp->message == WM_CREATE) {
            // Add "Always on Top" to the system menu when the window is created
            hSysMenu = GetSystemMenu(pwp->hwnd, FALSE);
            if (hSysMenu) {
                AppendMenu(hSysMenu, MF_STRING, ID_ALWAYS_ON_TOP, "Always on Top");
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HHOOK hHookCbt = NULL;
HHOOK hHookWnd = NULL;

// DLL entry point
BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        hInst = hInstance;
    }
    return TRUE;
}

// Install the hook
__declspec(dllexport) void InstallHook() {
    hHookCbt = SetWindowsHookEx(WH_CBT, HookProc, hInst, 0);
    hHookWnd = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, hInst, 0);
    if (hHookCbt && hHookWnd) {
        MessageBox(NULL, "Hook installed!", "Info", MB_OK);
    } else {
        MessageBox(NULL, "Failed to install hook!", "Error", MB_OK);
    }
}

// Uninstall the hook
__declspec(dllexport) void UninstallHook() {
    if (hHookWnd || hHookCbt) {
        UnhookWindowsHookEx(hHookWnd);
        UnhookWindowsHookEx(hHookCbt);
        hHookCbt = NULL;
        hHookWnd = NULL;
        MessageBox(NULL, "Hook uninstalled!", "Info", MB_OK);
    }
}

