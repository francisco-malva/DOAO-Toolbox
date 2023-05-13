#pragma once

#include <d3d9.h>

namespace MainMenu {
	void Init(IDirect3DDevice9* pDevice);
	void Draw();

	bool IsActive();
}
