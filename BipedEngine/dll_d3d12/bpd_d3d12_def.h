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