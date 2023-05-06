#include "FreeCam.h"

#include "Globals.h"
#include "CameraData.h"
#include "Transform.h"


bool FreeCam::IsActive;

D3DXVECTOR3 position;
D3DXVECTOR3 rotation;

void FreeCam::Update()
{
	if (!FreeCam::IsActive)
		return;

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

	if (GetAsyncKeyState('Q') & 1) {
		position.y -= 0.1f;
	}
	if (GetAsyncKeyState('E') & 1) {
		position.y += 0.1f;
	}

	Transform transform{};

	transform.Rotate(rotation.x, rotation.y, rotation.z).Translate(position.x, position.y, position.z);


	*CameraData::ViewMatrix = *transform.GetMatrix();
}
