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

		struct Buffer {
			Vertex *vertex;
			int vertex_count;
			WORD *index;
			int index_count;
		};
	}


	//************************************
	// Method:    dll_dllInfo
	// FullName:  dll_dllInfo
	// Access:    public 
	// Returns:   DLL const char * __stdcall
	// Qualifier:
	//************************************
	DLL const char * __stdcall dll_info();


	//************************************
	// Method:    dll_release
	// FullName:  dll_release
	// Access:    public 
	// Returns:   DLL void _stdcall
	// Qualifier:
	// Parameter: D3D11_Window & d3d11
	//************************************
	DLL void _stdcall dll_release(bpd::D3D11_Window& d3d11);


	//************************************
	// Method:    dll_createGraphics
	// FullName:  dll_createGraphics
	// Access:    public 
	// Returns:   DLL int __stdcall
	// Qualifier:
	// Parameter: bpd::D3D11_Window & d3d11
	// Parameter: HWND & hWnd
	//************************************
	DLL int __stdcall dll_createGraphics(
		bpd::D3D11_Window& d3d11,
		HWND& hWnd
	);


	//************************************
	// Method:    dll_updateGraphics
	// FullName:  dll_updateGraphics
	// Access:    public 
	// Returns:   DLL void __stdcall
	// Qualifier:
	// Parameter: bpd::D3D11_Window & d3d11
	// Parameter: float clear_color[4]
	// Parameter: int index
	//************************************
	DLL void __stdcall dll_updateGraphics(
		bpd::D3D11_Window &d3d11,
		float clear_color[4]
	);


	//************************************
	// Method:    dll_passBuffers
	// FullName:  dll_passBuffers
	// Access:    public 
	// Returns:   DLL void __stdcall
	// Qualifier:
	// Parameter: bpd::D3D11_Window & d3d11
	// Parameter: bpd::Vertex vertex[]
	// Parameter: int vertex_count
	// Parameter: WORD index[]
	// Parameter: int index_count
	//************************************
	DLL void __stdcall dll_passBuffers(
		bpd::D3D11_Window &d3d11,
		bpd::Buffer buffer[],
		int buffer_count
	);

#ifdef __cplusplus
}
#endif

#endif