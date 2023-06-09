#include <imgui.h>

#include "GameFlowControl.h"
#include "ScheduledFibers.h"
#include "BattleVars.h"

float GameFlowControl::GameSpeed = 1.0f;
bool GameFlowControl::IsPaused = false;

int GameFlowControl::FrameStepTimer = 0, GameFlowControl::DesiredFrameStep = 1;

float* GameSpeedGamePtr = reinterpret_cast<float*>(0x0290767c);
float* CameraSpeedGamePtr = reinterpret_cast<float*>(0x02908888);
float* ActualSpeedGamePtr = reinterpret_cast<float*>(0x02d58474);
bool* IsPausedPtr = reinterpret_cast<bool*>(0x02d5851b);

bool IsPausedBak;


typedef bool(*_cdecl CheckPlayerHasHitOther)(int);

CheckPlayerHasHitOther oCheckPlayerHasHitOther = reinterpret_cast<CheckPlayerHasHitOther>(0x00b67290);

bool ShouldBePaused() {
	return GameFlowControl::IsPaused && GameFlowControl::FrameStepTimer == 0;
}

void GameFlowControl::Update()
{

	if (!BattleVars::InBattle())
		return;


	*IsPausedPtr = ShouldBePaused();

	*GameSpeedGamePtr = *CameraSpeedGamePtr = *ActualSpeedGamePtr = GameSpeed;

	if (FrameStepTimer > 0) {
		--FrameStepTimer;

		if (!FrameStepTimer) {
			oCheckPlayerHasHitOther(0);
			oCheckPlayerHasHitOther(1);
		}
	}
}

void GameFlowControl::BeginFrameStep()
{
	FrameStepTimer = DesiredFrameStep;
}

void GameFlowControl::DrawMainUi()
{
    if (ImGui::CollapsingHeader("Game Flow Control")) {
        ImGui::SliderFloat("Game Speed", &GameFlowControl::GameSpeed, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
        ImGui::Checkbox("Is Paused", &GameFlowControl::IsPaused);

        if (GameFlowControl::IsPaused) {

            bool stepFrames = ImGui::Button("Step Frames");
            ImGui::SameLine();
            ImGui::InputInt("", &GameFlowControl::DesiredFrameStep);

            if (GameFlowControl::DesiredFrameStep < 1) {
                GameFlowControl::DesiredFrameStep = 1;
            }

            if (GameFlowControl::DesiredFrameStep > 999) {
                GameFlowControl::DesiredFrameStep = 999;
            }

            if (stepFrames) {
                GameFlowControl::BeginFrameStep();
            }
        }
    }
}
