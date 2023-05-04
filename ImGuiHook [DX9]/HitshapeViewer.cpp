
#include <d3dx9.h>

#include "HitshapeViewer.h"
#include "Shapes.h"
#include "Globals.h"
#include "CameraData.h"

struct Capsule {
	D3DXVECTOR4 p1, p2;
};

HitshapeViewer::PLAYER_COLLIDER_TYPE* PlayerHitTypeArray = reinterpret_cast<HitshapeViewer::PLAYER_COLLIDER_TYPE*>(0x028ff6bc);
HitshapeViewer::PLAYER_COLLIDER_TYPE* PlayerHurtTypeArray = reinterpret_cast<HitshapeViewer::PLAYER_COLLIDER_TYPE*>(0x028ff6f8);

unsigned char** PlayerCollisionIndicesPtrs = reinterpret_cast<unsigned char**>(0x029072e4);

D3DXVECTOR3* LastHit = reinterpret_cast<D3DXVECTOR3*>(0x02d59b18);

D3DXVECTOR4* PlayerHitSpherePosArray = reinterpret_cast<D3DXVECTOR4*>(0x028ff490);
D3DXVECTOR4* PlayerHurtSpherePosArray = reinterpret_cast<D3DXVECTOR4*>(0x028fed98);

Capsule* PlayerHurtCapsuleArray = reinterpret_cast<Capsule*>(0x028ff018);

float* PlayerHitSphereRadiiArray = reinterpret_cast<float*>(0x028ff418);
float* PlayerHurtSphereRadiiArray = reinterpret_cast<float*>(0x028fed18);
float* PlayerHurtCapsuleRadiiArray = reinterpret_cast<float*>(0x028fef98);

void DrawHitCollider(IDirect3DDevice9* pDevice, int playerId, int hitId);
void DrawHurtCollider(IDirect3DDevice9* pDevice, int playerId, int hitId);

void HitshapeViewer::Draw(IDirect3DDevice9* pDevice)
{

	if (PlayerCollisionIndicesPtrs[0] == NULL || PlayerCollisionIndicesPtrs[1] == NULL)
		return;
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);

	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);

	pDevice->SetTransform(D3DTS_VIEW, CameraData::ViewMatrix);
	pDevice->SetTransform(D3DTS_PROJECTION, CameraData::ProjMatrix);


	for (int playerId = 0; playerId < 2; playerId++) {

		int otherId = playerId == 0;

		if (Globals::DispHitSpheres) {
			for (int i = 0; i < 0xf; i++) {
				DrawHitCollider(pDevice, playerId, i);
			}
		}
		
		if (Globals::DispHurtSpheres) {
			for (int i = 0; i < 0x10; i++) {
				DrawHurtCollider(pDevice, playerId, i);
			}
		}
		
	}
}

void DrawHitCollider(IDirect3DDevice9* pDevice, int playerId, int hitId)
{
	HitshapeViewer::PLAYER_COLLIDER_TYPE colliderType = PlayerHitTypeArray[playerId * 0xf * 2 + PlayerCollisionIndicesPtrs[playerId][hitId] * 2];

	switch (colliderType)
	{
	case HitshapeViewer::PLAYER_COLLIDER_TYPE::PLAYER_COLLIDER_NONE:
		break;
	case HitshapeViewer::PLAYER_COLLIDER_TYPE::PLAYER_COLLIDER_CAPSULE:
		break;
	case HitshapeViewer::PLAYER_COLLIDER_TYPE::PLAYER_COLLIDER_SPHERE:
	{
		D3DXVECTOR4 spherePos = PlayerHitSpherePosArray[playerId * 0xF + hitId];
		float radius = PlayerHitSphereRadiiArray[playerId * 0xF + hitId];

		Shapes::DrawSphere(pDevice, { spherePos.x, spherePos.y, spherePos.z, radius }, D3DCOLOR_RGBA(255, 0, 0, 30));
	}
	break;
	default:
		break;
	}
}

void DrawHurtCollider(IDirect3DDevice9* pDevice, int playerId, int hitId)
{
	HitshapeViewer::PLAYER_COLLIDER_TYPE colliderType = PlayerHurtTypeArray[playerId * 0x10 * 2 + PlayerCollisionIndicesPtrs[playerId][hitId] * 2];

	switch (colliderType)
	{
	case HitshapeViewer::PLAYER_COLLIDER_TYPE::PLAYER_COLLIDER_NONE:
		break;
	case HitshapeViewer::PLAYER_COLLIDER_TYPE::PLAYER_COLLIDER_CAPSULE:
	{
		Capsule* capsule = &PlayerHurtCapsuleArray[playerId * 0x10 + hitId];
		float radius = PlayerHurtCapsuleRadiiArray[playerId * 0x10 + hitId];

		Shapes::DrawCapsule(
			pDevice, 
			{ capsule->p1.x, capsule->p1.y, capsule->p1.z },
			{ capsule->p2.x, capsule->p2.y, capsule->p2.z },
			radius, 
			D3DCOLOR_RGBA(0, 0, 255, 30));
	}
		break;
	case HitshapeViewer::PLAYER_COLLIDER_TYPE::PLAYER_COLLIDER_SPHERE:
	{
		D3DXVECTOR4 spherePos = PlayerHurtSpherePosArray[playerId * 0x10 + hitId];
		float radius = PlayerHurtSphereRadiiArray[playerId * 0x10 + hitId];

		Shapes::DrawSphere(pDevice, { spherePos.x, spherePos.y, spherePos.z, radius }, D3DCOLOR_RGBA(0, 0, 255, 30));
	}
	break;
	default:
		break;
	}
}
