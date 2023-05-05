#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

#include <windows.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>


#include "Menu.h"
#include "HitshapeViewer.h"
#include "Hook.h"
#include "Shapes.h"
#include "Globals.h"

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        Hook::Process.hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, Hook::Init, 0, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}