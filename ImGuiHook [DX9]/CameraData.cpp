#include "CameraData.h"

D3DMATRIX* CameraData::ViewMatrix = reinterpret_cast<D3DMATRIX*>(0x028fe850);
D3DMATRIX* CameraData::ProjMatrix = reinterpret_cast<D3DMATRIX*>(0x028fe810);