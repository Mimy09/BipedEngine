#ifndef _BPD_DLL_IMPORT_H_
#define _BPD_DLL_IMPORT_H_

#include "../math/bpd-vector.h"

#define _DLL_FUNCTIONS_N 4

#define dll_Load(hdll, x) bpd::dll::loadDll(hdll, x)
#define dll_Unload(hdll) bpd::dll::UnloadDll(hdll)
#define dll_CreateGraphics bpd::dll::functions.by_type.dll_createGraphics
#define dll_UpdateGraphics bpd::dll::functions.by_type.dll_updateGraphics
#define dll_PassBuffers bpd::dll::functions.by_type.dll_passBuffers
#define dll_DllInfo bpd::dll::functions.by_type.dll_dllInfo

#define Handle(name) typedef struct _##name* name


namespace bpd { namespace dll {
	Handle(HBPDD3D);
	typedef int(__stdcall *_dll_func_ptr)();

	const char* DLL_FUNCTION_NAMES[_DLL_FUNCTIONS_N] =
	{
		"_dll_createGraphics@8",
		"_dll_dllInfo@0",
		"_dll_passBuffers@20",
		"_dll_updateGraphics@12",
	};

	typedef struct {
		int(__stdcall* dll_createGraphics)(
			HBPDD3D& hD3D,
			HWND& hWnd
		);
		const char *(__stdcall* dll_dllInfo)();
		void(__stdcall* dll_passBuffers)(
			HBPDD3D& hD3D,
			Vertex_buffer vertex[],
			int vertex_count,
			WORD index[],
			int index_count
		);
		void(__stdcall* dll_updateGraphics)(
			HBPDD3D& hD3D,
			float clear_color[4],
			int index
		);

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