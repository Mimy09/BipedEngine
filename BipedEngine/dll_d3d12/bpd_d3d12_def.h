#pragma once


namespace bpd{
	struct _D3D12_Window{
		enum { FRAME = 3 }; // number of buffers we want, 2 for double buffering, 3 for triple buffering

		IDXGIFactory4* dxgiFactory; // factory
		ID3D12Debug* debugInterface; // debug layer

		ID3D12Device* device; // direct3d device
		IDXGISwapChain3* swapChain; // swapchain used to switch between render targets
		ID3D12CommandQueue* commandQueue; // container for command lists
		ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets
		ID3D12Resource* renderTargets[FRAME]; // number of render targets equal to buffer count
		ID3D12CommandAllocator* commandAllocator[FRAME]; // we want enough allocators for each buffer * number of threads (we only have one thread)
		ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame
		ID3D12Fence* fence[FRAME];    // an object that is locked while our command list is being executed by the GPU. We need as many 
												 //as we have allocators (more if we want to know when the GPU is finished with an asset)
		HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the GPU
		UINT64 fenceValue[FRAME]; // this value is incremented each frame. each fence will have its own value

		ID3D12PipelineState* pipelineStateObject; // PSO containing a pipeline state
		ID3D12RootSignature* rootSignature; // root signature defines data shaders will access
		D3D12_VIEWPORT viewport; // area that output from rasterizer will be stretched to.
		D3D12_RECT scissorRect; // the area to draw in. pixels outside that area will not be drawn onto

		ID3D12Resource* vertexBuffer; // a default buffer in GPU memory that we will load vertex data for our triangle into
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView; // a structure containing a pointer to the vertex data in GPU memory
												   // the total size of the buffer, and the size of each element (vertex)

		ID3D12Resource* indexBuffer; // a default buffer in GPU memory that we will load index data for our triangle into
		D3D12_INDEX_BUFFER_VIEW indexBufferView; // a structure holding information about the index buffer


		DXGI_SAMPLE_DESC sampleDesc;
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
		D3D12_SHADER_BYTECODE pixelShaderBytecode;
		D3D12_SHADER_BYTECODE vertexShaderBytecode;

		ID3D12Resource* depthStencilBuffer; // This is the memory for our depth buffer. it will also be used for a stencil buffer in a later tutorial
		ID3D12DescriptorHeap* dsDescriptorHeap; // This is a heap for our depth/stencil buffer descriptor

		ID3D12DescriptorHeap* mainDescriptorHeap[FRAME]; // this heap will store the descriptor to our constant buffer
		//ID3D12Resource* constantBufferUploadHeap[FRAME]; // this is the memory on the GPU where our constant buffer will be placed.

		// Constant buffers must be 256-byte aligned which has to do with constant reads on the GPU.
		// We are only able to read at 256 byte intervals from the start of a resource heap, so we will
		// make sure that we add padding between the two constant buffers in the heap (one for cube1 and one for cube2)
		// Another way to do this would be to add a float array in the constant buffer structure for padding. In this case
		// we would need to add a float padding[50]; after the wvpMat variable. This would align our structure to 256 bytes (4 bytes per float)
		// The reason i didn't go with this way, was because there would actually be wasted CPU cycles when memcpy our constant
		// buffer data to the GPU virtual address. currently we memcpy the size of our structure, which is 16 bytes here, but if we
		// were to add the padding array, we would memcpy 64 bytes if we memcpy the size of our structure, which is 50 wasted bytes
		// being copied.
		int ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;

		ConstantBufferPerObject cbPerObject; // this is the constant buffer data we will send to the GPU 
											 // (which will be placed in the resource we created above)

		ID3D12Resource* constantBufferUploadHeaps[FRAME]; // this is the memory on the GPU where constant buffers for each frame will be placed

		UINT8* cbvGPUAddress[FRAME]; // this is a pointer to each of the constant buffer resource heaps

		DirectX::XMFLOAT4X4 cameraProjMat; // this will store our projection matrix
		DirectX::XMFLOAT4X4 cameraViewMat; // this will store our view matrix

		DirectX::XMFLOAT4 cameraPosition; // this is our cameras position vector
		DirectX::XMFLOAT4 cameraTarget; // a vector describing the point in space our camera is looking at
		DirectX::XMFLOAT4 cameraUp; // the worlds up vector

		std::vector<Transform> obj_list;

		int frameIndex; // current rtv we are on
		int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
							   // function declarations

		// width and height of the window
		int Width = 800;
		int Height = 600;

		
		bool FullScreen = false;

		// we will exit the program when this becomes false
		bool Running = true;

	};
}