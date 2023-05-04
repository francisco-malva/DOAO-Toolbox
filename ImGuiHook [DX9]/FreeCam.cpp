#include "FreeCam.h"

#include "Globals.h"
#include "CameraData.h"

D3DXVECTOR3 position;
D3DXVECTOR3 rotation;

void FreeCam::Update()
{
	if (!Globals::IsFreeCamera)
		return;

	D3DXMATRIX trans, rot, f;

	float x = cosf(D3DXToRadian(rotation.y + 90.0f)), z = sinf(D3DXToRadian(rotation.y + 90.0f));

	if (GetAsyncKeyState('W') & 1) {
		position -= D3DXVECTOR3(x, 0.0f, z) * 0.1f;
	}
	if (GetAsyncKeyState('S') & 1) {
		position += D3DXVECTOR3(x, 0.0f, z) * 0.1f;
	}
	if (GetAsyncKeyState('A') & 1) {
		rotation.y += 0.1f;
		rotation.y += D3DX_PI * 2.0f;
		rotation.y = fmodf(rotation.y, D3DX_PI * 2.0f);
	}

	if (GetAsyncKeyState('D') & 1) {
		rotation.y -= 0.1f;
		rotation.y += D3DX_PI * 2.0f;
		rotation.y = fmodf(rotation.y, D3DX_PI * 2.0f);
	}

	if (GetAsyncKeyState(VK_UP) & 1) {
		position.y -= 0.1f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 1) {
		position.y += 0.1f;
	}

	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&rot, rotation.y, rotation.x, rotation.z);

	D3DXMatrixMultiply(&f, &rot, &trans);

	*CameraData::ViewMatrix = f;
}
