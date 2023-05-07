#include "GameFlowControl.h"


float GameFlowControl::GameSpeed = 1.0f;
bool GameFlowControl::IsPaused = false;

bool* IsPausedGamePtr = reinterpret_cast<bool*>(0x02d5851b);

float* GameSpeedGamePtr = reinterpret_cast<float*>(0x0290767c);
float* CameraSpeedGamePtr = reinterpret_cast<float*>(0x02908888);
float* ActualSpeedGamePtr = reinterpret_cast<float*>(0x02d58474);

void GameFlowControl::Update()
{
	if (IsPaused != *IsPausedGamePtr) {
		*IsPausedGamePtr = IsPaused;
	}

	if (GameSpeed != *GameSpeedGamePtr) {
		*GameSpeedGamePtr = GameSpeed;
		*CameraSpeedGamePtr = GameSpeed;
		*ActualSpeedGamePtr = GameSpeed;
	}
}
