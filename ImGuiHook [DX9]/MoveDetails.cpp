#include <imgui.h>

#include "MainMenu.h"
#include "MoveDetails.h"

bool MoveDetails::IsOpen;

float* DistanceBetweenPlayersPtr = reinterpret_cast<float*>(0x02d58600);


USHORT* CurrentMovePtrs[2] = {
	reinterpret_cast<USHORT*>(0x02d62f70 + 64),
	reinterpret_cast<USHORT*>(0x02d63018 + 64)
};

USHORT* NextMovePtrs[2] = {
	reinterpret_cast<USHORT*>(0x02d62f70 + 66),
	reinterpret_cast<USHORT*>(0x02d63018 + 66)
};

USHORT* MoveStartupFramesPtr = reinterpret_cast<USHORT*>(0x0288ecf0);
USHORT* MoveActiveFramesPtr = reinterpret_cast<USHORT*>(0x0288ecec);
USHORT* MoveRecoveryFramesPtr = reinterpret_cast<USHORT*>(0x0288ece8);

int DesiredMoveIds[2];
bool ForcedMoveIds[2];

void MoveDetails::DrawMainUi()
{
	if (ImGui::Button("Show Move Details")) {
		MoveDetails::IsOpen = true;
	}
}

void MoveDetails::DrawWindow()
{
	if (!IsOpen || !MainMenu::IsActive())
		return;

	ImGui::Begin("Move Details", &IsOpen);

	if (ImGui::CollapsingHeader("Common")) {
		ImGui::Text("Distance: %.3f", *DistanceBetweenPlayersPtr);
	}

	
	for (size_t i = 0; i < 2; i++)
	{
		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		if (ImGui::CollapsingHeader(i == 0 ? "P1" : "P2")) {
			ImGui::PushID(i);

			ImGui::Text("Current Move ID: %04hx", *CurrentMovePtrs[i]);
			ImGui::Text("Frame Data: %hu (%hu) %hu", MoveStartupFramesPtr[i], MoveActiveFramesPtr[i], MoveRecoveryFramesPtr[i]);

			ImGui::Dummy(ImVec2(0, 20.0f));

			bool setMoveId = ImGui::Button("Set Move ID");
			ImGui::Checkbox("Force Move Id", &ForcedMoveIds[i]);
			if (ImGui::InputInt("Move ID", &DesiredMoveIds[i], 1, 100, ImGuiInputTextFlags_CharsHexadecimal)) {
				if (DesiredMoveIds[i] < 0)
				{
					DesiredMoveIds[i] = 0;
				}
				if (DesiredMoveIds[i] > USHRT_MAX) {
					DesiredMoveIds[i] = USHRT_MAX;
				}
			}

			

			setMoveId |= ForcedMoveIds[i];

			if (setMoveId && *CurrentMovePtrs[i] == 0) {
				*NextMovePtrs[i] = DesiredMoveIds[i];
			}
			ImGui::PopID();
		}

		
	}
	ImGui::End();
}
