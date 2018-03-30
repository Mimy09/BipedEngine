#include "bpd_opengl.h"
#include "bpd_opengl_def.h"

using namespace bpd;

#define Assert(x) if (!(x)) { MessageBoxA(0, #x, "Assertion Failed", MB_OK); __debugbreak(); }
#define Check(x) if (!(x)) { MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak(); }
#define CheckSucceeded(hresult) Check(SUCCEEDED(hresult))


#ifdef __cplusplus
extern "C" {
#endif

const char * __stdcall dll_info(){
	return "BPD_OPENGL";
}

void _stdcall dll_release(OPENGL_Window& d3d11){
	
}

int __stdcall dll_createGraphics(OPENGL_Window& opengl, HWND& hWnd){
	
	return 0;
}

void __stdcall dll_updateGraphics(OPENGL_Window &opengl, float clear_color[4]) {

}

void __stdcall dll_passBuffers(OPENGL_Window &opengl, Buffer buffer[], int buffer_count) {
	
}

#ifdef __cplusplus
}
#endif