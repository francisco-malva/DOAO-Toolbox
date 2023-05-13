#pragma once

#include <d3d9.h>

namespace HitshapeViewer {

	enum class PLAYER_COLLIDER_TYPE : unsigned char {
		PLAYER_COLLIDER_NONE = 0x0,
		PLAYER_COLLIDER_CAPSULE = 0x3,
		PLAYER_COLLIDER_SPHERE = 0x1
	};

	enum HITSHAPE_DISPLAY_FLAGS {
		HITSHAPE_DISP_NONE = 0x0,
		HITSHAPE_DISP_HIT = 0x1,
		HITSHAPE_DISP_HURT = 0x2
	};

	extern bool DispHitSpheres;
	extern bool DispHurtSpheres;

	void DrawCollision(IDirect3DDevice9* pDevice);

	void DrawMainUi();
}

