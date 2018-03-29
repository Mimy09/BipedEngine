#ifndef _BPD_DLL_IMPORT_H_
#define _BPD_DLL_IMPORT_H_

#include "../math/bpd-vector.h"

#define _DLL_FUNCTIONS_N 3

#define dll_Load(hdll, x) bpd::dll::loadDll(hdll, x)
#define dll_Unload(hdll) bpd::dll::UnloadDll(hdll)
#define dll_CreateGraphics(hd3d,hWnd) bpd::dll::functions.by_type.dll_createGraphics(hd3d,hWnd)
#define dll_UpdateGraphics(hd3d) bpd::dll::functions.by_type.dll_updateGraphics(hd3d)
#define dll_DllInfo() bpd::dll::functions.by_type.dll_dllInfo()

#define Handle(name) typedef struct _##name* name


namespace bpd { namespace dll {
	Handle(HBPDD3D);
	typedef int(__stdcall *_dll_func_ptr)();

	const char* DLL_FUNCTION_NAMES[_DLL_FUNCTIONS_N] =
	{
		"_dll_createGraphics@8",
		"_dll_dllInfo@0",
		"_dll_updateGraphics@4",
	};

	typedef struct {
		int(__stdcall* dll_createGraphics)(
			HBPDD3D& hD3D,
			HWND& hWnd
		);
		const char *(__stdcall* dll_dllInfo)();
		void(__stdcall* dll_updateGraphics)(HBPDD3D& hD3D);
	} functions_struct;

	typedef union {
		functions_struct  by_type;
		_dll_func_ptr     func_ptr[_DLL_FUNCTIONS_N];
	} functions_union;

	static functions_union functions;

	bool loadDll(HINSTANCE &hDLL, const char* dllname) {
		hDLL = LoadLibrary(dllname);
		if (!hDLL) return false;

		for(int i = 0; i < _DLL_FUNCTIONS_N; i++) {
			functions.func_ptr[i] = (_dll_func_ptr)::GetProcAddress(hDLL,DLL_FUNCTION_NAMES[i]);
			Check_(functions.func_ptr[i],DLL_FUNCTION_NAMES[i]);
		}

		return true;
	}

	void UnloadDll(HINSTANCE &hDLL){
		if (hDLL != NULL) FreeLibrary(hDLL);
	}
} }
#endif