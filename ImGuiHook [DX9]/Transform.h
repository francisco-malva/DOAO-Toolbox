#pragma once
#include <d3dx9.h>

class Transform
{
private:
	D3DXMATRIX matrix_;
public:
	Transform();
	Transform& Translate(float x, float y, float z);
	Transform& Rotate(float x, float y, float z);
	Transform& Scale(float x, float y, float z);
	Transform& LookAtLH(const D3DXVECTOR3 pEye, const D3DXVECTOR3 pAt, const D3DXVECTOR3 pUp);
	Transform& LookAtRH(const D3DXVECTOR3 pEye, const D3DXVECTOR3 pAt, const D3DXVECTOR3 pUp);

	D3DXMATRIX* GetMatrix();
};

