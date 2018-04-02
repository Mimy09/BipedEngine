#ifndef _BPD_OPENGL_H_
#define _BPD_OPENGL_H_

#define Handle(name) typedef struct _##name* name
#define DLL __declspec(dllexport)

/** Includes **************************************************************/
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

	namespace bpd {
		Handle(OPENGL_Window);

		struct Vertex {
			//glm::vec3 pos;
			//glm::vec4 col;
		};

		struct Buffer {
			Vertex *vertex;
			int vertex_count;
			WORD *index;
			int index_count;
		};

	}

	DLL const char * __stdcall dll_info();

	DLL void _stdcall dll_release(bpd::OPENGL_Window& d3d11);

	DLL int __stdcall dll_createGraphics(
		bpd::OPENGL_Window& opengl,
		HWND& hWnd
	);

	DLL void __stdcall dll_update(
		bpd::OPENGL_Window &opengl
	);

	DLL void __stdcall dll_draw(
		bpd::OPENGL_Window &opengl,
		bpd::Buffer buffer[],
		int buffer_count,
		float clear_color[4]
	);

#ifdef __cplusplus
}
#endif

#endif