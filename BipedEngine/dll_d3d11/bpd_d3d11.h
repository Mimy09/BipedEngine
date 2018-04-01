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

#include <chrono>

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

		struct Buffer {
			Vertex *vertex;
			int vertex_count;
			WORD *index;
			int index_count;
		};
	}

	DLL const char * __stdcall dll_info();

	DLL void _stdcall dll_release(
		bpd::D3D11_Window& d3d11
	);

	DLL int __stdcall dll_createGraphics(
		bpd::D3D11_Window& d3d11,
		HWND& hWnd
	);

	DLL void __stdcall dll_update(
		bpd::D3D11_Window &d3d11
	);

	DLL void __stdcall dll_draw(
		bpd::D3D11_Window &d3d11,
		bpd::Buffer buffer[],
		int buffer_count,
		float clear_color[4]
	);

#ifdef __cplusplus
}
#endif

#endif