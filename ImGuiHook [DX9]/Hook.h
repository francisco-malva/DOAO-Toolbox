#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <detours.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

namespace Hook {


struct CurrentProcess {
	HMODULE hModule;
	HWND TargetHwnd;
	WNDPROC TargetWndProc;
};

struct _DirectXVersion {
	int Unknown = 0;
	int D3D9 = 1;
	int D3D10 = 2;
	int D3D11 = 3;
	int D3D12 = 4;
} ;

typedef HRESULT(APIENTRY* EndScene) (IDirect3DDevice9*);
typedef HRESULT(APIENTRY* Present) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
typedef HRESULT(APIENTRY* DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
typedef HRESULT(APIENTRY* DrawPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT);
typedef HRESULT(APIENTRY* SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
typedef HRESULT(APIENTRY* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
typedef HRESULT(APIENTRY* SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
typedef HRESULT(APIENTRY* SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
typedef HRESULT(APIENTRY* SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
typedef HRESULT(APIENTRY* SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
typedef HRESULT(APIENTRY* SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);
typedef HRESULT(APIENTRY* SetTransform)(IDirect3DDevice9*, D3DTRANSFORMSTATETYPE, D3DMATRIX*);
typedef HRESULT(APIENTRY* GetTransform)(IDirect3DDevice9*, D3DTRANSFORMSTATETYPE, D3DMATRIX*);

extern CurrentProcess Process;
extern _DirectXVersion DirectXVersion;


DWORD WINAPI Init(LPVOID lpParameter);
bool CheckDirectXVersion(int version);

}