#include <imgui.h>

#include "MainMenu.h"
#include "MoveDetails.h"

enum PLAYER_HIT_LEVEL : SHORT {
	PLAYER_HIT_LEVEL_OTHER = 0,
	PLAYER_HIT_LEVEL_HIGH = 1,
	PLAYER_HIT_LEVEL_MID = 2,
	PLAYER_HIT_LEVEL_LOW = 3
};


enum PLAYER_STATE : UINT8 {
	PLAYER_STATE_IDLE = 0,
	PLAYER_STATE_JUMPING = 1,
	PLAYER_STATE_JUMPING_STRIKE = 2,
	PLAYER_STATE_STRIKE = 3,
	PLAYER_STATE_THROWING = 4,
	PLAYER_STATE_ATTEMPT_HOLD = 5,
	PLAYER_STATE_HOLDING = 6,
	PLAYER_STATE_BLOCKING = 7,
	PLAYER_STATE_HURT_HOLDABLE = 8,
	PLAYER_STATE_HURT_UNHOLDABLE = 9,
	PLAYER_STATE_THROWN = 10,
	PLAYER_STATE_HELD = 11,
	PLAYER_STATE_LYING_DOWN = 12,
	PLAYER_STATE_OTHER = 13,
	PLAYER_STATE_DOWNED_ATTACK = 14,
	PLAYER_STATE_ATTEMPTING_THROW = 16
};

enum PLAYER_STRIKE_TYPE : UINT8 {
	PLAYER_STRIKE_TYPE_HIGH_P = 0,
	PLAYER_STRIKE_TYPE_HIGH_K = 1,
	PLAYER_STRIKE_TYPE_MID_P = 2,
	PLAYER_STRIKE_TYPE_MID_K = 3,
	PLAYER_STRIKE_TYPE_LOW_P = 4,
	PLAYER_STRIKE_TYPE_LOW_K = 5
};

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

PLAYER_STRIKE_TYPE* PlayerStrikeTypesPtr[2] = {
	reinterpret_cast<PLAYER_STRIKE_TYPE*>(0x02d62f70 + 0x69),
	reinterpret_cast<PLAYER_STRIKE_TYPE*>(0x02d63018 + 0x69)
};

PLAYER_STATE* PlayerStatesPtr[2] = {
	reinterpret_cast<PLAYER_STATE*>(0x02d62f70 + 0x45),
	reinterpret_cast<PLAYER_STATE*>(0x02d63018 + 0x45)
};

USHORT* MoveStartupFramesPtr = reinterpret_cast<USHORT*>(0x0288ecf0);
USHORT* MoveActiveFramesPtr = reinterpret_cast<USHORT*>(0x0288ecec);
USHORT* MoveRecoveryFramesPtr = reinterpret_cast<USHORT*>(0x0288ece8);

int DesiredMoveIds[2];
bool ForcedMoveIds[2];



const char* StrikeTypeToStr(PLAYER_STRIKE_TYPE type);
const char* PlayerStateToStr(PLAYER_STATE state);

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


			ImGui::Text("Current State: %s", PlayerStateToStr(*PlayerStatesPtr[i]));

			const char* hitLevel = nullptr;

			switch (*PlayerStatesPtr[i])
			{
			default:
				break;
			}
			ImGui::Text("Hit Level: %s", StrikeTypeToStr(*PlayerStrikeTypesPtr[i]));
			
			ImGui::Dummy(ImVec2(0, 20.0f));

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

const char* StrikeTypeToStr(PLAYER_STRIKE_TYPE type)
{
	switch (type) {
	case PLAYER_STRIKE_TYPE_HIGH_P:
		return "High P";
	case PLAYER_STRIKE_TYPE_HIGH_K:
		return "High K";
	case PLAYER_STRIKE_TYPE_LOW_K:
		return "Low K";
	case PLAYER_STRIKE_TYPE_LOW_P:
		return "Low P";
	case PLAYER_STRIKE_TYPE_MID_K:
		return "Mid K";
	case PLAYER_STRIKE_TYPE_MID_P:
		return "Mid P";
	}
}

const char* PlayerStateToStr(PLAYER_STATE state)
{
	switch (state) {
	case PLAYER_STATE_ATTEMPTING_THROW:
		return "Attempting Throw";
	case PLAYER_STATE_ATTEMPT_HOLD:
		return "Attempting Hold";
	case PLAYER_STATE_BLOCKING:
		return "Blocking";
	case PLAYER_STATE_DOWNED_ATTACK:
		return "Downed Attack";
	case PLAYER_STATE_HELD:
		return "Held";
	case PLAYER_STATE_HOLDING:
		return "Holding";
	case PLAYER_STATE_HURT_HOLDABLE:
		return "Stunned Holdable";
	case PLAYER_STATE_HURT_UNHOLDABLE:
		return "Stunned Unholdable";
	case PLAYER_STATE_IDLE:
		return "Idle";
	case PLAYER_STATE_JUMPING:
		return "Jumping";
	case PLAYER_STATE_JUMPING_STRIKE:
		return "Jumping Strike";
	case PLAYER_STATE_LYING_DOWN:
		return "Lying Down";
	case PLAYER_STATE_OTHER:
		return "Other";
	case PLAYER_STATE_STRIKE:
		return "Striking";
	case PLAYER_STATE_THROWING:
		return "Throwing";
	case PLAYER_STATE_THROWN:
		return "Thrown";
	}
}
