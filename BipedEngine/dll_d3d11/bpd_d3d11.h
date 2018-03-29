#ifndef _BPD_D3D11_H_
#define _BPD_D3D11_H_

/** Includes **************************************************************/
#include <stdio.h>
#include <d3d11.h>
#include <dxgi1_5.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define Handle(name) typedef struct _##name* name

#define DLL __declspec(dllexport)

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
	namespace bpd {
		Handle(D3D11_Window);

		struct Vertex {
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Col;
		};
	}


	DLL const char * __stdcall dll_dllInfo();

	DLL int __stdcall dll_createGraphics(
		bpd::D3D11_Window& d3d11,
		HWND& hWnd
	);

	DLL void __stdcall dll_updateGraphics(bpd::D3D11_Window &d3d11);

#ifdef __cplusplus
}
#endif

#endif