#pragma once

#include <Windows.h>



namespace ScheduledFibers
{
	struct ScheduledFiber {
		DWORD Flags;
		int YieldFrames;
		LPVOID FiberHandle;
		LPVOID Entrypoint;
		LARGE_INTEGER Performance;
		LPVOID OnDelete;
	};

    typedef enum FIBER_ID {
        FIBER_ID_MAIN_PROCESSING = 0,
        FIBER_ID_GAMEFLOW_PROCESSING = 1,
        FIBER_ID_UNUSED1 = 2,
        FIBER_ID_UNUSED2 = 3,
        FIBER_ID_UNUSED4 = 4,
        FIBER_ID_HEALTH_PROCESSING = 5,
        FIBER_ID_PLAYER_ACT_UPDATE = 6,
        FIBER_ID_PLAYER_ACT2_UPDATE = 7,
        FIBER_ID_GAME_PHYSICS_UPDATE = 8,
        FIBER_ID_ANIMATION_UPDATE = 9,
        FIBER_ID_PLAYER_STRIKE_COLLISION = 10,
        FIBER_ID_CAMERA_BEHAVIOR = 11,
        FIBER_ID_SETUP_DRAWING = 12,
        FIBER_ID_STAGE_DRAWING = 13,
        FIBER_ID_PLAYER_DRAWING = 14,
        FIBER_ID_TERMINATOR = 255
    } FIBER_ID;

	extern ScheduledFiber* GameFibersPtr;
};

