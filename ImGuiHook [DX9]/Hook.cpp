
#include <imgui_impl_dx9.h>

#include "Hook.h"
#include "Globals.h"
#include "Menu.h"
#include "HitshapeViewer.h"
#include "WidescreenFix.h"
#include "StagePropDrawer.h"
#include "FreeCam.h"
#include "CameraData.h"
#include "Shapes.h"
#include "GameFlowControl.h"

bool Initialized;

Hook::CurrentProcess Hook::Process;
Hook::_DirectXVersion  Hook::DirectXVersion;

Hook::BeginScene oBeginScene;
Hook::EndScene oEndScene;
Hook::Present oPresent;
Hook::DrawIndexedPrimitive oDrawIndexedPrimitive;
Hook::DrawPrimitive oDrawPrimitive;
Hook::SetTexture oSetTexture;
Hook::Reset oReset;
Hook::SetStreamSource oSetStreamSource;
Hook::SetVertexDeclaration oSetVertexDeclaration;
Hook::SetVertexShaderConstantF oSetVertexShaderConstantF;
Hook::SetVertexShader oSetVertexShader;
Hook::SetPixelShader oSetPixelShader;
Hook::SetTransform oSetTransform;
Hook::GetTransform oGetTransform;

void DrawToolbox(IDirect3DDevice9* pDevice);

HRESULT APIENTRY hkBeginScene(IDirect3DDevice9* pDevice) {

    if (pDevice == NULL)
        return oBeginScene(pDevice);

    return oBeginScene(pDevice);
}

HRESULT APIENTRY hkEndScene(IDirect3DDevice9* pDevice) {
    if (pDevice == NULL)
        return oEndScene(pDevice);

    if (!Initialized) {

        Menu::Init(pDevice);

        Initialized = true;
    }

    WidescreenFix::Update();
    GameFlowControl::Update();

    DrawToolbox(pDevice);
    
    return oEndScene(pDevice);
}



HRESULT APIENTRY hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
    if (pDevice == NULL)
        return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

    return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT APIENTRY hkDrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
    if (pDevice == NULL)
        return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

    return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY hkDrawPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    if (pDevice == NULL)
        return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);

    return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkSetTexture(IDirect3DDevice9* pDevice, DWORD Stage, IDirect3DBaseTexture9* pTexture) {
    if (pDevice == NULL)
        return oSetTexture(pDevice, Stage, pTexture);

    return oSetTexture(pDevice, Stage, pTexture);
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    if (pDevice == NULL)
        return oReset(pDevice, pPresentationParameters);

    D3DDEVICE_CREATION_PARAMETERS pParameters{ 0 };
    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto ResetReturn = oReset(pDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();
    return ResetReturn;
}

HRESULT APIENTRY hkSetStreamSource(IDirect3DDevice9* pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) {
    if (pDevice == NULL)
        return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);

    return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT APIENTRY hkSetVertexDeclaration(IDirect3DDevice9* pDevice, IDirect3DVertexDeclaration9* pDecl) {
    if (pDevice == NULL)
        return oSetVertexDeclaration(pDevice, pDecl);

    return oSetVertexDeclaration(pDevice, pDecl);
}

HRESULT APIENTRY hkSetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, const float* pConstantData, UINT Vector4fCount) {
    if (pDevice == NULL)
        return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);

    return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY hkSetVertexShader(IDirect3DDevice9* pDevice, IDirect3DVertexShader9* pShader) {
    if (pDevice == NULL)
        return oSetVertexShader(pDevice, pShader);

    return oSetVertexShader(pDevice, pShader);
}

HRESULT APIENTRY hkSetPixelShader(IDirect3DDevice9* pDevice, IDirect3DPixelShader9* pShader) {
    if (pDevice == NULL)
        return oSetPixelShader(pDevice, pShader);

    return oSetPixelShader(pDevice, pShader);
}

HRESULT APIENTRY hkSetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) {

    if (pDevice == NULL)
        return oSetTransform(pDevice, State, pMatrix);

    if (pMatrix == CameraData::ViewMatrix) {
        FreeCam::Update();
    }
    
    return oSetTransform(pDevice, State, pMatrix);
}

HRESULT APIENTRY hkGetTransform(IDirect3DDevice9* pDevice, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) {

    return oGetTransform(pDevice, State, pMatrix);
}

DWORD WINAPI Hook::Init(LPVOID lpParameter)
{
    if (CheckDirectXVersion(DirectXVersion.D3D9) == false) {
        return FALSE;
    }

    HWND WindowHwnd = CreateWindow("BUTTON", "DX", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Process.hModule, NULL);
    if (WindowHwnd == NULL) {
        return FALSE;
    }

    IDirect3D9* IDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (IDirect3D9 == NULL) {
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

    D3DPRESENT_PARAMETERS pParameters;
    ZeroMemory(&pParameters, sizeof(pParameters));
    pParameters.Windowed = TRUE;
    pParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pParameters.hDeviceWindow = WindowHwnd;
    pParameters.BackBufferFormat = D3DFMT_UNKNOWN;

    IDirect3DDevice9* pDevice;
    if (IDirect3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pParameters, &pDevice) != D3D_OK) {
        IDirect3D9->Release();
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

    if (pDevice == NULL) {
        IDirect3D9->Release();
        DestroyWindow(WindowHwnd);
        return FALSE;
    }

#if defined _M_X64
    DWORD64* DVTable = (DWORD64*)pDevice;
    DVTable = (DWORD64*)DVTable[0];
#elif defined _M_IX86
    DWORD* DVTable = (DWORD*)pDevice;
    DVTable = (DWORD*)DVTable[0];
#endif

    oBeginScene = (BeginScene)DVTable[41];
    oEndScene = (EndScene)DVTable[42];
    oSetTransform = (SetTransform)DVTable[44];
    oGetTransform = (GetTransform)DVTable[45];
    oPresent = (Present)DVTable[17];
    oDrawIndexedPrimitive = (DrawIndexedPrimitive)DVTable[82];
    oDrawPrimitive = (DrawPrimitive)DVTable[81];
    oSetTexture = (SetTexture)DVTable[65];
    oReset = (Reset)DVTable[16];
    oSetStreamSource = (SetStreamSource)DVTable[100];
    oSetVertexDeclaration = (SetVertexDeclaration)DVTable[87];
    oSetVertexShaderConstantF = (SetVertexShaderConstantF)DVTable[94];
    oSetVertexShader = (SetVertexShader)DVTable[92];
    oSetPixelShader = (SetPixelShader)DVTable[107];

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(LPVOID&)oBeginScene, (PBYTE)hkBeginScene);
    DetourAttach(&(LPVOID&)oEndScene, (PBYTE)hkEndScene);
    DetourAttach(&(LPVOID&)oPresent, (PBYTE)hkPresent);
    DetourAttach(&(LPVOID&)oDrawIndexedPrimitive, (PBYTE)hkDrawIndexedPrimitive);
    DetourAttach(&(LPVOID&)oDrawPrimitive, (PBYTE)hkDrawPrimitive);
    DetourAttach(&(LPVOID&)oSetTexture, (PBYTE)hkSetTexture);
    DetourAttach(&(LPVOID&)oReset, (PBYTE)hkReset);
    DetourAttach(&(LPVOID&)oSetStreamSource, (PBYTE)hkSetStreamSource);
    DetourAttach(&(LPVOID&)oSetVertexDeclaration, (PBYTE)hkSetVertexDeclaration);
    DetourAttach(&(LPVOID&)oSetVertexShaderConstantF, (PBYTE)hkSetVertexShaderConstantF);
    DetourAttach(&(LPVOID&)oSetVertexShader, (PBYTE)hkSetVertexShader);
    DetourAttach(&(LPVOID&)oSetPixelShader, (PBYTE)hkSetPixelShader);
    DetourAttach(&(LPVOID&)oSetTransform, (PBYTE)hkSetTransform);
    DetourAttach(&(LPVOID&)oGetTransform, (PBYTE)hkGetTransform);
    DetourTransactionCommit();

    pDevice->Release();
    IDirect3D9->Release();
    DestroyWindow(WindowHwnd);
    return TRUE;
}

bool Hook::CheckDirectXVersion(int version)
{
	if (version = DirectXVersion.D3D12) {
		if (GetModuleHandle("d3d12.dll") != NULL) {
			return true;
		}
	}

	if (version = DirectXVersion.D3D11) {
		if (GetModuleHandle("d3d11.dll") != NULL) {
			return true;
		}
	}

	if (version = DirectXVersion.D3D10) {
		if (GetModuleHandle("d3d10.dll") != NULL) {
			return true;
		}
	}

	if (version = DirectXVersion.D3D9) {
		if (GetModuleHandle("d3d9.dll") != NULL) {
			return true;
		}
	}

	return false;
}

void DrawToolbox(IDirect3DDevice9* pDevice)
{
    IDirect3DStateBlock9* state;

    pDevice->CreateStateBlock(D3DSBT_ALL, &state);
    state->Capture();

    Menu::Draw();
    HitshapeViewer::Draw(pDevice);
    StagePropDrawer::Draw(pDevice);
    Shapes::DrawDebugTriangle(pDevice);

    state->Apply();
    state->Release();
}
