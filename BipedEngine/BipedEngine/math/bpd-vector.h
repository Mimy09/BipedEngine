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

	struct Buffer {
		Vertex_buffer* vertex;
		int vertex_count;
		DWORD* index;
		int index_count;
	};

	static Vertex_buffer vBoxList[] = {
		// front face
		Vec3{ -0.5f,  0.5f, -0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
		Vec3{ 0.5f, -0.5f, -0.5f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ -0.5f, -0.5f, -0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ 0.5f,  0.5f, -0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },

		// right side face
		Vec3{ 0.5f, -0.5f, -0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
		Vec3{ 0.5f,  0.5f,  0.5f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ 0.5f, -0.5f,  0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ 0.5f,  0.5f, -0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },

		// left side face
		Vec3{ -0.5f,  0.5f,  0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
		Vec3{ -0.5f, -0.5f, -0.5f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ -0.5f, -0.5f,  0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ -0.5f,  0.5f, -0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },

		// back face
		Vec3{ 0.5f,  0.5f,  0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
		Vec3{ -0.5f, -0.5f,  0.5f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ 0.5f, -0.5f,  0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ -0.5f,  0.5f,  0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },

		// top face
		Vec3{ -0.5f,  0.5f, -0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
		Vec3{ 0.5f,  0.5f,  0.5f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ 0.5f,  0.5f, -0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ -0.5f,  0.5f,  0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },

		// bottom face
		Vec3{ 0.5f, -0.5f,  0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f },
		Vec3{ -0.5f, -0.5f, -0.5f }, Vec4{ 1.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ 0.5f, -0.5f, -0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		Vec3{ -0.5f, -0.5f,  0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f },
	};//24

	  // a quad (2 triangles)
	static DWORD iBoxList[] = {
		// front face
		0, 1, 2, // first triangle
		0, 3, 1, // second triangle

				 // left face
				 4, 5, 6, // first triangle
				 4, 7, 5, // second triangle

						  // right face
						  8, 9, 10, // first triangle
						  8, 11, 9, // second triangle

									// back face
									12, 13, 14, // first triangle
									12, 15, 13, // second triangle

												// top face
												16, 17, 18, // first triangle
												16, 19, 17, // second triangle

															// bottom face
															20, 21, 22, // first triangle
															20, 23, 21, // second triangle
	};//36

	static Buffer boxBuffer[] { {vBoxList, 24, iBoxList, 36} }; 
}

#endif