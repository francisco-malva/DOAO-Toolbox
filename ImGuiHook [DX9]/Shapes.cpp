#include <vector>

#include <d3dx9.h>

#define PAR_OCTASPHERE_IMPLEMENTATION
#include "par_octasphere.h"

#include "Shapes.h"
#include "Transform.h"


struct ColoredVertex {
	D3DXVECTOR3 position;
	D3DCOLOR color;
};

std::vector<uint16_t> indices;

std::vector<float> unswizzeledVerts;
std::vector<ColoredVertex> swizzeledVerts;

std::vector<ColoredVertex> lineVerts;

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


	Transform transform{};

	pDevice->SetTransform(D3DTS_WORLD, transform.Translate(pos.x, pos.y, pos.z).GetMatrix());
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

	Transform transform{};


	float pitch = 0.0f;
	float yaw = 0.0f;

	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;
	float lenXZsquare = dx * dx + dz * dz;

	float lenSquare = lenXZsquare + dy * dy;

	pitch = atan2f(-sqrt(lenXZsquare), dy);
	yaw = atan2f(-dx, dz);


	float mx = a.x + b.x;
	float my = a.y + b.y;
	float mz = a.z + b.z;

	mx *= 0.5f;
	my *= 0.5f;
	mz *= 0.5f;

	transform.
		Rotate(pitch, yaw, 0.0f).Translate(mx, my, mz);


	pDevice->SetTransform(D3DTS_WORLD, transform.GetMatrix());
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->LightEnable(0, FALSE);
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, mesh.num_vertices, mesh.num_indices / 3, mesh.indices, D3DFMT_INDEX16, swizzeledVerts.data(), sizeof(ColoredVertex));
}


void Shapes::AddLine(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DCOLOR color) {

	lineVerts.emplace_back(ColoredVertex({ a,color }));
	lineVerts.emplace_back(ColoredVertex({ b,color }));
}

void Shapes::DrawLines(IDirect3DDevice9* pDevice) {

	D3DXMATRIX mat;

	D3DXMatrixIdentity(&mat);

	pDevice->SetTransform(D3DTS_WORLD, &mat);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->LightEnable(0, FALSE);


	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, lineVerts.size() / 2, lineVerts.data(), sizeof(ColoredVertex));

	lineVerts.clear();

}
