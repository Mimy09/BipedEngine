#ifndef _BPD_OPENGL_H_
#define _BPD_OPENGL_H_



#define Handle(name) typedef struct _##name* name
#define DLL __declspec(dllexport)

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	namespace bpd {
		Handle(OPENGL_Window);
	}

	DLL const char * __stdcall dll_dllInfo();

	DLL int __stdcall dll_createGraphics(
		bpd::OPENGL_Window& d3d11,
		HWND& hWnd
	);

	DLL void __stdcall dll_updateGraphics(bpd::OPENGL_Window &d3d11);

#ifdef __cplusplus
}
#endif

#endif