#include <vector>

#include <d3dx9.h>

#define PAR_OCTASPHERE_IMPLEMENTATION
#include "par_octasphere.h"

#include "Shapes.h"


struct ColoredVertex {
	D3DXVECTOR3 position;
	D3DCOLOR color;
};

std::vector<uint16_t> indices;
std::vector<float> unswizzeledVerts;
std::vector<ColoredVertex> swizzeledVerts;


void Shapes::DrawSphere(IDirect3DDevice9* pDevice, D3DXVECTOR4 pos, D3DCOLOR color)
{
	par_octasphere_config cfg{};

	cfg.corner_radius = pos.w;
	cfg.height = 0.0f;
	cfg.width = 0.0f;
	cfg.depth = 0.0f;
	cfg.num_subdivisions = 4;

	uint32_t num_indices, num_verts;

	par_octasphere_get_counts(&cfg, &num_indices, &num_verts);


	unswizzeledVerts.resize(num_verts * 3);
	swizzeledVerts.resize(num_verts);
	indices.resize(num_indices);

	par_octasphere_mesh mesh{};

	mesh.indices = indices.data();
	mesh.positions = unswizzeledVerts.data();

	par_octasphere_populate(&cfg, &mesh);


	for (size_t i = 0; i < num_verts; i++) {
		swizzeledVerts[i].position = { unswizzeledVerts[i * 3], unswizzeledVerts[i * 3 + 1], unswizzeledVerts[i * 3 + 2] };
		swizzeledVerts[i].color = color;
	}

	D3DXMATRIX mat;

	D3DXMatrixTranslation(&mat, pos.x, pos.y, pos.z);

	pDevice->SetTransform(D3DTS_WORLD, &mat);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->LightEnable(0, FALSE);
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, mesh.num_vertices, mesh.num_indices / 3, mesh.indices, D3DFMT_INDEX16,swizzeledVerts.data(), sizeof(ColoredVertex));
}

void Shapes::DrawCapsule(IDirect3DDevice9* pDevice, D3DXVECTOR3 a, D3DXVECTOR3 b, float radius, D3DCOLOR color)
{
	par_octasphere_config cfg{};

	D3DXVECTOR3 dir, axis;

	D3DXVec3Subtract(&dir, &b, &a);

	float length = D3DXVec3Length(&dir);

	cfg.corner_radius = radius;
	cfg.height = length;
	cfg.width = 0.0f;
	cfg.depth = 0.0f;
	cfg.num_subdivisions = 4;

	uint32_t num_indices, num_verts;

	par_octasphere_get_counts(&cfg, &num_indices, &num_verts);


	unswizzeledVerts.resize(num_verts * 3);
	swizzeledVerts.resize(num_verts);
	indices.resize(num_indices);

	par_octasphere_mesh mesh{};

	mesh.indices = indices.data();
	mesh.positions = unswizzeledVerts.data();

	par_octasphere_populate(&cfg, &mesh);


	for (size_t i = 0; i < num_verts; i++) {
		swizzeledVerts[i].position = { unswizzeledVerts[i * 3], unswizzeledVerts[i * 3 + 1], unswizzeledVerts[i * 3 + 2] };
		swizzeledVerts[i].color = color;
	}

	D3DXMATRIX local, trans, rotT, rot, f;


	D3DXMatrixTranslation(&local, 0.0f, length / 2.0f, 0.0f);

	D3DXVECTOR3 zero = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 forward = { 0.0f,0.0f,1.0f };
	D3DXVec3Normalize(&axis, &dir);

	D3DXMatrixLookAtRH(&rotT, &zero, &forward, &axis);

	D3DXMatrixTranslation(&trans, a.x, a.y, a.z);

	D3DXMatrixMultiply(&rot, &local, &rotT);
	D3DXMatrixMultiply(&f, &rot, &trans);

	pDevice->SetTransform(D3DTS_WORLD, &f);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->LightEnable(0, FALSE);
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, mesh.num_vertices, mesh.num_indices / 3, mesh.indices, D3DFMT_INDEX16, swizzeledVerts.data(), sizeof(ColoredVertex));
}
