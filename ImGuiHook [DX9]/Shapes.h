#pragma once

#include <d3d9.h>
#include <d3dx9.h>

namespace Shapes {

	void DrawSphere(IDirect3DDevice9* pDevice, D3DXVECTOR4 pos, D3DCOLOR color);

	void DrawCapsule(IDirect3DDevice9* pDevice, D3DXVECTOR3 a, D3DXVECTOR3 b, float radius, D3DCOLOR color);
}

