#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

#include "MainMenu.h"
#include "MoveDetails.h"
#include "Hook.h"
#include "Globals.h"
#include "HitshapeViewer.h"
#include "FreeCam.h"
#include "GameFlowControl.h"
#include "MainMenu.h"
#include "Visibility.h"

bool ShowMenu;

void InputHandler() {
    for (int i = 0; i < 5; i++) {
        ImGui::GetIO().MouseDown[i] = false;
    }

    int Button = -1;
    if (GetAsyncKeyState(VK_LBUTTON)) {
        Button = 0;
    }

    if (Button != -1) {
        ImGui::GetIO().MouseDown[Button] = true;
    }
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }
    return CallWindowProc(Hook::Process.TargetWndProc, hWnd, msg, wParam, lParam);
}

void MainMenu::Init(IDirect3DDevice9* pDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    D3DDEVICE_CREATION_PARAMETERS pParameters;
    pDevice->GetCreationParameters(&pParameters);
    Hook::Process.TargetHwnd = pParameters.hFocusWindow;

    if (Hook::Process.TargetHwnd != NULL) {
        Hook::Process.TargetWndProc = (WNDPROC)SetWindowLongPtr(Hook::Process.TargetHwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
        ImGui_ImplWin32_Init(Hook::Process.TargetHwnd);
        ImGui_ImplDX9_Init(pDevice);
        ImGui::GetIO().ImeWindowHandle = Hook::Process.TargetHwnd;
    }
}

void MainMenu::Draw()
{
    if (GetAsyncKeyState(VK_INSERT) & 1)
        ShowMenu = !ShowMenu;

    ImGui::GetIO().MouseDrawCursor = ShowMenu;


    if (ShowMenu) {
        ImGui::Begin("DOAO Toolbox", &ShowMenu, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

        HitshapeViewer::DrawMainUi();
        
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        //TODO: Refactor
        if (ImGui::CollapsingHeader("Experimental")) {
            ImGui::Checkbox("Widescreen", &Globals::IsWidescreen);
            ImGui::Checkbox("Free Camera", &FreeCam::IsActive);
        }

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        GameFlowControl::DrawMainUi();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        Visibility::DrawUi();

        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        MoveDetails::DrawMainUi();

        ImGui::End();
    }

   

}

bool MainMenu::IsActive()
{
    return ShowMenu;
}
