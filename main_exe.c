#include <windows.h>
#include <stdio.h>

// Define the function pointer types for the DLL functions
typedef void (*InstallHookFunc)();
typedef void (*UninstallHookFunc)();

HINSTANCE hLib;

// Function to load and hook the DLL
void LoadDLLAndHook() {
    hLib = LoadLibrary("TopMostDll.dll");
    if (!hLib) {
        printf("Failed to load DLL\n");
        return;
    }

    // Get pointers to the hook functions
    InstallHookFunc InstallHook = (InstallHookFunc)GetProcAddress(hLib, "InstallHook");
    if (!InstallHook) {
        printf("Failed to get InstallHook function\n");
        return;
    }

    // Install the hook
    InstallHook();
    printf("Hook installed!\n");
}

// Function to unload the DLL and unhook
void UnloadDLLAndUnhook() {
    if (hLib) {
        UninstallHookFunc UninstallHook = (UninstallHookFunc)GetProcAddress(hLib, "UninstallHook");
        if (UninstallHook) {
            UninstallHook();
            printf("Hook uninstalled!\n");
        }

        FreeLibrary(hLib);
        hLib = NULL;
    }
}

int main() {
    int choice;

    while (1) {
        printf("1. Install Hook\n2. Uninstall Hook\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                LoadDLLAndHook();
                break;
            case 2:
                UnloadDLLAndUnhook();
                break;
            case 3:
                UnloadDLLAndUnhook();
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}

