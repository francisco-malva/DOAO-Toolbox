#pragma once

namespace GameFlowControl
{
	extern float GameSpeed;
	extern bool IsPaused;
	extern int FrameStepTimer, DesiredFrameStep;

	void Update();

	void BeginFrameStep();

	void DrawMainUi();
};

