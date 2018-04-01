#ifndef _BPD_OPENGL_H_
#define _BPD_OPENGL_H_

#define Handle(name) typedef struct _##name* name
#define DLL __declspec(dllexport)

/** Includes **************************************************************/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#if defined(CreateWindow)
#undef CreateWindow
#endif

#include <wrl.h>
using namespace Microsoft::WRL;

#include <stdio.h>
#pragma warning(disable:4996)

// D3D12 specific headers
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include "d3dx12.h"

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

// STL Headers
#include <algorithm>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

	namespace bpd {
		Handle(D3D12_Window);

		struct Vertex {
			//Vertex(float x,float y,float z,float r,float g,float b,float a) : pos(x,y,z),col(r,g,b,z) {}
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 col;
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
		bpd::D3D12_Window& d3d12
	);

	DLL int __stdcall dll_createGraphics(
		bpd::D3D12_Window& d3d12,
		HWND& hWnd
	);

	DLL void __stdcall dll_update(
		bpd::D3D12_Window &d3d12
	);

	DLL void __stdcall dll_draw(
		bpd::D3D12_Window &d3d12,
		bpd::Buffer buffer[],
		int buffer_count,
		float clear_color[4]
	);

#ifdef __cplusplus
}
#endif

#endif