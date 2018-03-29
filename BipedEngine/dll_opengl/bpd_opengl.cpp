#include "bpd_opengl.h"

using namespace bpd;

const char * __stdcall dll_dllInfo(){
	return "BPD_OPENGL";
}

int __stdcall dll_createGraphics(OPENGL_Window& d3d11, HWND& hWnd){
	return 0;
}

void __stdcall dll_updateGraphics(OPENGL_Window &d3d11){
	
}