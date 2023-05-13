#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include "GeneralUi.h"

void GeneralUi::BeginFrame()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void GeneralUi::EndFrame()
{
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
