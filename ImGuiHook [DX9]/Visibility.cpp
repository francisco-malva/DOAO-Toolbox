#include <imgui.h>

#include "Visibility.h"
#include "ScheduledFibers.h"

int* StageDrawingFiberStatusPtr = reinterpret_cast<int*>(0x02908ae8 + 0xd * 0x1c);

void Visibility::DrawUi()
{
	ScheduledFibers::ScheduledFiber* fiber = ScheduledFibers::GameFibersPtr + ScheduledFibers::FIBER_ID_SETUP_DRAWING;

	if (ImGui::CollapsingHeader("Visibility")) {
		if (ImGui::Button("Toggle Stage Visibility")) {

			if ((*StageDrawingFiberStatusPtr & 1) == 0) {
				*StageDrawingFiberStatusPtr = 1;
			}
			else {
				*StageDrawingFiberStatusPtr = 0;
			}
		}
	}

}
