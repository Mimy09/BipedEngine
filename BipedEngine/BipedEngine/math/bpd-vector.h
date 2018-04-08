#ifndef _BPD_VECTOR_H_
#define _BPD_VECTOR_H_

namespace bpd{
	struct Vec3{
		//Vec3(float _x,float _y,float _z) : x(_x), y(_y), z(_z){}
		float x, y, z;
	};
	struct Vec4{
		//Vec4(float _x,float _y,float _z, float _w) : x(_x),y(_y),z(_z),w(_w) {}
		float x, y, z, w;
	};

	struct Vertex_buffer {
		//Vertex_buffer(float x,float y,float z,float r,float g,float b,float a) : pos(x,y,z),col(r,g,b,z) {}
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
		{ Vec3{-0.5f,  0.5f, 0.5f }, Vec4{ 1.0f, 0.0f, 0.0f, 1.0f } }, // top left
		{ Vec3{ 0.5f, -0.5f, 0.5f }, Vec4{ 0.0f, 1.0f, 0.0f, 1.0f } }, // bottom right
		{ Vec3{-0.5f, -0.5f, 0.5f }, Vec4{ 0.0f, 0.0f, 1.0f, 1.0f } }, // bottom left
		{ Vec3{ 0.5f,  0.5f, 0.5f }, Vec4{ 0.0f, 0.0f, 0.0f, 1.0f } }, // top right
	}; // 4
	static DWORD iBoxList[] = {
		0, 1, 2, // first triangle
		0, 3, 1, // second triangle
	}; // 6

	static Vertex_buffer vCubeList[] =
	{
		{Vec3{-1.0f, -1.0f, -1.0f}, Vec4{1.0f, 0.0f, 0.0f, 1.0f}},
		{Vec3{-1.0f, +1.0f, -1.0f}, Vec4{0.0f, 1.0f, 0.0f, 1.0f}},
		{Vec3{+1.0f, +1.0f, -1.0f}, Vec4{0.0f, 0.0f, 1.0f, 1.0f}},
		{Vec3{+1.0f, -1.0f, -1.0f}, Vec4{1.0f, 1.0f, 0.0f, 1.0f}},
		{Vec3{-1.0f, -1.0f, +1.0f}, Vec4{0.0f, 1.0f, 1.0f, 1.0f}},
		{Vec3{-1.0f, +1.0f, +1.0f}, Vec4{1.0f, 1.0f, 1.0f, 1.0f}},
		{Vec3{+1.0f, +1.0f, +1.0f}, Vec4{1.0f, 0.0f, 1.0f, 1.0f}},
		{Vec3{+1.0f, -1.0f, +1.0f}, Vec4{1.0f, 0.0f, 0.0f, 1.0f}},
	};

	DWORD iCubeList[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	static Buffer cubeBuffer[] { { vCubeList, 8, iCubeList, 36} };
	static Buffer boxBuffer[]{ { vBoxList, 4, iBoxList, 6 } };
}

#endif