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
#include <vector>

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
			Vertex(float x,float y,float z,float r,float g,float b,float a) : pos(x,y,z),col(r,g,b,z) {}
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 col;
		};

		struct Buffer {
			Vertex *vertex;
			int vertex_count;
			DWORD *index;
			int index_count;
		};

		// this is the structure of our constant buffer.
		struct ConstantBufferPerObject {
			DirectX::XMFLOAT4X4 wvpMat;
		};

		struct Transform{
			DirectX::XMFLOAT4X4 WorldMat; // our cubes world matrix (transformation matrix)
			DirectX::XMFLOAT4X4 RotMat; // this will keep track of our rotation for the cube
			DirectX::XMFLOAT4 Position; // our cubes position in space
			DirectX::XMFLOAT4 Scale; // our cubes position in space
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

	DLL void __stdcall dll_reloadBuffers(
		bpd::D3D12_Window &d3d12,
		bpd::Buffer buffer[],
		int buffer_count
	);

	DLL void __stdcall dll_setView(
		bpd::D3D12_Window &d3d12,
		DirectX::XMFLOAT4 cam_pos,
		DirectX::XMFLOAT4 cam_target,
		float fov
	);

	DLL int __stdcall dll_createObj(
		bpd::D3D12_Window &d3d12,
		DirectX::XMFLOAT4 pos
	);

	DLL void __stdcall dll_transformObj(
		bpd::D3D12_Window &d3d12,
		int ID,
		DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 rotation,
		DirectX::XMFLOAT4 scale
	);

#ifdef __cplusplus
}
#endif

#endif