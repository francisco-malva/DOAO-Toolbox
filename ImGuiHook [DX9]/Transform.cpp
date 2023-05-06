#include "Transform.h"

Transform::Transform()
{
    D3DXMatrixIdentity(&matrix_);
}

Transform& Transform::Translate(float x, float y, float z)
{
    D3DXMATRIX mat;

    D3DXMatrixTranslation(&mat, x, y, z);

    matrix_ *= mat;

    return *this;
}

Transform& Transform::Rotate(float x, float y, float z)
{
    D3DXMATRIX mat;

    D3DXMatrixRotationYawPitchRoll(&mat, y, x, z);

    matrix_ *= mat;


    return *this;
}

Transform& Transform::Scale(float x, float y, float z)
{
    D3DXMATRIX mat;

    D3DXMatrixScaling(&mat, x, y, z);

    matrix_ *= mat;

    return *this;
}

Transform& Transform::LookAtLH(const D3DXVECTOR3 pEye, const D3DXVECTOR3 pAt, const D3DXVECTOR3 pUp)
{
    D3DXMATRIX mat;
    D3DXMatrixLookAtLH(&mat, &pEye, &pAt, &pUp);

    matrix_ *= mat;

    return *this;
}

Transform& Transform::LookAtRH(const D3DXVECTOR3 pEye, const D3DXVECTOR3 pAt, const D3DXVECTOR3 pUp)
{
    D3DXMATRIX mat;
    D3DXMatrixLookAtRH(&mat, &pEye, &pAt, &pUp);

    matrix_ *= mat;
    return *this;
}

D3DXMATRIX* Transform::GetMatrix()
{
    return &matrix_;
}
