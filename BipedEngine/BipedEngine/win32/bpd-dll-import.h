#ifndef _BPD_DLL_IMPORT_H_
#define _BPD_DLL_IMPORT_H_

#include "../math/bpd-vector.h"

#define _DLL_FUNCTIONS_N 9

#define dll_Load bpd::dll::loadDll
#define dll_Unload bpd::dll::UnloadDll

#define dll_CreateGraphics bpd::dll::functions.by_type.dll_createGraphics
#define dll_Update bpd::dll::functions.by_type.dll_update
#define dll_Release bpd::dll::functions.by_type.dll_release
#define dll_Draw bpd::dll::functions.by_type.dll_draw
#define dll_ReloadBuffers bpd::dll::functions.by_type.dll_reloadBuffers
#define dll_SetView bpd::dll::functions.by_type.dll_setView
#define dll_Info bpd::dll::functions.by_type.dll_info

#define dll_CreateObj bpd::dll::functions.by_type.dll_createObj
#define dll_TransformObj bpd::dll::functions.by_type.dll_transformObj


#define Handle(name) typedef struct _##name* name


namespace bpd { namespace dll {
	Handle(HBPDD3D);
	typedef int(__stdcall *_dll_func_ptr)();

	const char* DLL_FUNCTION_NAMES[_DLL_FUNCTIONS_N] =
	{
#if defined _WIN64
		"dll_createGraphics",
		"dll_createObj",
		"dll_draw",
		"dll_info",
		"dll_release",
		"dll_reloadBuffers",
		"dll_setView",
		"dll_transformObj",
		"dll_update",
#else
		"_dll_createGraphics@8",
		"_dll_createObj@20",
		"_dll_draw@16",
		"_dll_info@0",
		"_dll_release@4",
		"_dll_reloadBuffers@12",
		"_dll_setView@40",
		"_dll_transformObj@56",
		"_dll_update@4",
#endif
	};

	typedef struct {
		int(__stdcall* dll_createGraphics)(
			HBPDD3D& hD3D,
			HWND& hWnd
		);
		int (__stdcall* dll_createObj)(
			HBPDD3D &d3d12,
			Vec4 pos
		);
		void(__stdcall* dll_draw)(
			HBPDD3D& hD3D,
			Buffer buffer[],
			int buffer_count,
			float clear_color[4]
		);
		const char *(__stdcall* dll_info)();
		void(__stdcall* dll_release)(
			HBPDD3D& hD3D
		);
		void *(__stdcall* dll_reloadBuffers)(
			HBPDD3D &d3d12,
			Buffer buffer[],
			int buffer_count
		);
		void *(__stdcall* dll_setView)(
			HBPDD3D &d3d12,
			Vec4 cam_pos,
			Vec4 cam_target,
			float fov
		);
		void (__stdcall* dll_transformObj)(
			HBPDD3D &d3d12,
			int ID,
			Vec4 position,
			Vec4 rotation,
			Vec4 scale
		);
		void(__stdcall* dll_update)(
			HBPDD3D& hD3D
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