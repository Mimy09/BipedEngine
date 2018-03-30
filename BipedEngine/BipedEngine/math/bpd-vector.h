#ifndef _BPD_VECTOR_H_
#define _BPD_VECTOR_H_

namespace bpd{
	struct Vec2{ float x, y; };
	struct Vec3{ float x, y, z; };
	struct Vec4{ float x, y, z, w; };

	struct Vertex_buffer {
		Vec3 pos;
		Vec4 col;
	};
}

#endif