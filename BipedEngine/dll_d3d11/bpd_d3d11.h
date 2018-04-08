#ifndef _BPD_D3D11_H_
#define _BPD_D3D11_H_

/** Includes **************************************************************/
#include <stdio.h>
#include <d3d11.h>
#include <dxgi1_5.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <D2D1.h>
#include <sstream>
#include <dwrite.h>
#include <dinput.h>

#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")


#define Handle(name) typedef struct _##name* name
#define DLL __declspec(dllexport)

#include <windows.h>
#include <chrono>
#include <vector>

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
			DWORD *index;
			int index_count;
		};

		struct ConstantBufferPerObject {
			DirectX::XMFLOAT4X4 wvpMat;
		};

		struct Transform {
			DirectX::XMFLOAT4X4 World;
			DirectX::XMFLOAT4X4 Rotation;

			DirectX::XMFLOAT4 Position;
			DirectX::XMFLOAT4 Scale;
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

	DLL void __stdcall dll_reloadBuffers(
		bpd::D3D11_Window &d3d11,
		bpd::Buffer buffer[],
		int buffer_count
	);

	DLL void __stdcall dll_setView(
		bpd::D3D11_Window &d3d11,
		DirectX::XMFLOAT4 cam_pos,
		DirectX::XMFLOAT4 cam_target,
		float fov
	);

	DLL int __stdcall dll_createObj(
		bpd::D3D11_Window &d3d11,
		DirectX::XMFLOAT4 pos
	);

	DLL void __stdcall dll_transformObj(
		bpd::D3D11_Window &d3d11,
		int ID,
		DirectX::XMFLOAT4 position,
		DirectX::XMFLOAT4 rotation,
		DirectX::XMFLOAT4 scale
	);

#ifdef __cplusplus
}
#endif

#endif