#include "bpd_d3d12.h"
#include "bpd_d3d12_def.h"

using namespace bpd;

#define Assert(x) if (!(x)) { MessageBoxA(0, #x, "Assertion Failed", MB_OK); __debugbreak(); }
#define Check(x) if (!(x)) { MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak(); }
#define Check_(x, msg) if (!(x)) { MessageBoxA(0, msg, "Check Failed", MB_OK); __debugbreak(); }
#define CheckSucceeded(hresult) Check(SUCCEEDED(hresult))
#define CheckSucceeded_(hresult, msg) Check_(SUCCEEDED(hresult), msg)


#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#ifdef __cplusplus
extern "C" {
#endif
	
	bool InitD3D(D3D12_Window &d3d12, HWND hwnd, Vertex *vList, int vertex_count, DWORD* iList, int index_count) {
		HRESULT hr;
		int vBufferSize = vertex_count * sizeof(Vertex);
		int iBufferSize = index_count * sizeof(DWORD);

		/********************************************************************************************************************/
		// -- Create the Device -- //
		{
			CheckSucceeded( CreateDXGIFactory1(IID_PPV_ARGS(&d3d12->dxgiFactory)));

			IDXGIAdapter1* adapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

			int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

									   // find first hardware gpu that supports d3d 12
			while(d3d12->dxgiFactory->EnumAdapters1(adapterIndex,&adapter) != DXGI_ERROR_NOT_FOUND) {
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
					// we dont want a software device
					continue;
				}

				// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
				hr = D3D12CreateDevice(adapter,D3D_FEATURE_LEVEL_11_0,_uuidof(ID3D12Device),nullptr);
				if(SUCCEEDED(hr)) {
					break;
				}

				adapterIndex++;
			}

			// Create the device
			CheckSucceeded(D3D12CreateDevice(
				adapter,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&d3d12->device)
			));
		}
		/********************************************************************************************************************/
		// -- Create a direct command queue -- //
		{
			D3D12_COMMAND_QUEUE_DESC cqDesc = {};
			cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // direct means the gpu can directly execute this command queue

			CheckSucceeded(d3d12->device->CreateCommandQueue(&cqDesc,IID_PPV_ARGS(&d3d12->commandQueue))); // create the command queue
		}

		/********************************************************************************************************************/
		// -- Create the Swap Chain (double/tripple buffering) -- //
		{
			DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
			backBufferDesc.Width = d3d12->Width; // buffer width
			backBufferDesc.Height = d3d12->Height; // buffer height
			backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)

																// describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
			d3d12->sampleDesc = {};
			d3d12->sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)

								  // Describe and create the swap chain.
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount = d3d12->FRAME; // number of buffers we have
			swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
			swapChainDesc.OutputWindow = hwnd; // handle to our window
			swapChainDesc.SampleDesc = d3d12->sampleDesc; // our multi-sampling description
			swapChainDesc.Windowed = !d3d12->FullScreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

			IDXGISwapChain* tempSwapChain;

			d3d12->dxgiFactory->CreateSwapChain(
				d3d12->commandQueue, // the queue will be flushed once the swap chain is created
				&swapChainDesc, // give it the swap chain description we created above
				&tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
			);

			d3d12->swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

			d3d12->frameIndex = d3d12->swapChain->GetCurrentBackBufferIndex();
		}
		/********************************************************************************************************************/
		// -- Create the Back Buffers (render target views) Descriptor Heap -- //
		{
			// describe an rtv descriptor heap and create
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = d3d12->FRAME; // number of descriptors for this heap.
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

															   // This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
															   // otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			CheckSucceeded(d3d12->device->CreateDescriptorHeap(&rtvHeapDesc,IID_PPV_ARGS(&d3d12->rtvDescriptorHeap)));

			// get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
			// descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
			// device to give us the size. we will use this size to increment a descriptor handle offset
			d3d12->rtvDescriptorSize = d3d12->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			// get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
			// but we cannot literally use it like a c++ pointer.
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(d3d12->rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
			for(int i = 0; i < d3d12->FRAME; i++) {
				// first we get the n'th buffer in the swap chain and store it in the n'th
				// position of our ID3D12Resource array
				CheckSucceeded(d3d12->swapChain->GetBuffer(i,IID_PPV_ARGS(&d3d12->renderTargets[i])));

				// the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
				d3d12->device->CreateRenderTargetView(d3d12->renderTargets[i],nullptr,rtvHandle);

				// we increment the rtv handle by the rtv descriptor size we got above
				rtvHandle.Offset(1,d3d12->rtvDescriptorSize);
			}
		}
		/********************************************************************************************************************/
		// -- Create the Command Allocators -- //
		{
			for(int i = 0; i < d3d12->FRAME; i++) {
				CheckSucceeded(d3d12->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&d3d12->commandAllocator[i])));
			}
		}
		/********************************************************************************************************************/
		// -- Create a Command List -- //
		{
			// create the command list with the first allocator
			CheckSucceeded(d3d12->device->CreateCommandList(0,D3D12_COMMAND_LIST_TYPE_DIRECT,d3d12->commandAllocator[d3d12->frameIndex],NULL,IID_PPV_ARGS(&d3d12->commandList)));
		}
		/********************************************************************************************************************/
		// -- Create a Fence & Fence Event -- //
		{
			// create the fences
			for(int i = 0; i < d3d12->FRAME; i++) {
				CheckSucceeded(d3d12->device->CreateFence(0,D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(&d3d12->fence[i])));

				d3d12->fenceValue[i] = 0; // set the initial fence value to 0
			}

			// create a handle to a fence event
			d3d12->fenceEvent = CreateEvent(nullptr,FALSE,FALSE,nullptr);
			if(d3d12->fenceEvent == nullptr) {
				return false;
			}
		}
		/********************************************************************************************************************/
		// -- create root signature -- //
		{
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0,nullptr,0,nullptr,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ID3DBlob* signature;
			CheckSucceeded(D3D12SerializeRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1,&signature,nullptr));

			CheckSucceeded(d3d12->device->CreateRootSignature(0,signature->GetBufferPointer(),signature->GetBufferSize(),IID_PPV_ARGS(&d3d12->rootSignature)));
		}
		/********************************************************************************************************************/
		// -- create vertex and pixel shaders -- //
		{
			// when debugging, we can compile the shader files at runtime.
			// but for release versions, we can compile the hlsl shaders
			// with fxc.exe to create .cso files, which contain the shader
			// bytecode. We can load the .cso files at runtime to get the
			// shader bytecode, which of course is faster than compiling
			// them at runtime

			// compile vertex shader
			ID3DBlob* vertexShader; // d3d blob for holding vertex shader bytecode
			ID3DBlob* errorBuff; // a buffer holding the error data if any
			CheckSucceeded_(D3DCompileFromFile(L"VertexShader.hlsl",
				nullptr,
				nullptr,
				"main",
				"vs_5_0",
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0,
				&vertexShader,
				&errorBuff),
			(char*)errorBuff->GetBufferPointer());

			// fill out a shader bytecode structure, which is basically just a pointer
			// to the shader bytecode and the size of the shader bytecode
			d3d12->vertexShaderBytecode = {};
			d3d12->vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
			d3d12->vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();
		}
		{
			// compile pixel shader
			ID3DBlob* pixelShader;
			ID3DBlob* errorBuff; // a buffer holding the error data if any
			CheckSucceeded_(D3DCompileFromFile(L"PixelShader.hlsl",
				nullptr,
				nullptr,
				"main",
				"ps_5_0",
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0,
				&pixelShader,
				&errorBuff),(char*)errorBuff->GetBufferPointer());

			// fill out shader bytecode structure for pixel shader
			d3d12->pixelShaderBytecode = {};
			d3d12->pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
			d3d12->pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();
		}
		/********************************************************************************************************************/
		// -- create input layout -- //
		{
			// The input layout is used by the Input Assembler so that it knows
			// how to read the vertex data bound to it.

			D3D12_INPUT_ELEMENT_DESC inputLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			// fill out an input layout description structure
			d3d12->inputLayoutDesc = {};

			// we can get the number of elements in an array by "sizeof(array) / sizeof(arrayElementType)"
			d3d12->inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
			d3d12->inputLayoutDesc.pInputElementDescs = inputLayout;
		}
		/********************************************************************************************************************/
		// -- create a pipeline state object (PSO) -- //
		{
			// In a real application, you will have many pso's. for each different shader
			// or different combinations of shaders, different blend states or different rasterizer states,
			// different topology types (point, line, triangle, patch), or a different number
			// of render targets you will need a pso

			// VS is the only required shader for a pso. You might be wondering when a case would be where
			// you only set the VS. It's possible that you have a pso that only outputs data with the stream
			// output, and not on a render target, which means you would not need anything after the stream
			// output.

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; // a structure to define a pso
			psoDesc.InputLayout = d3d12->inputLayoutDesc; // the structure describing our input layout
			psoDesc.pRootSignature = d3d12->rootSignature; // the root signature that describes the input data this pso needs
			psoDesc.VS = d3d12->vertexShaderBytecode; // structure describing where to find the vertex shader bytecode and how large it is
			psoDesc.PS = d3d12->pixelShaderBytecode; // same as VS but for pixel shader
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
			psoDesc.SampleDesc = d3d12->sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
			psoDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blent state.
			psoDesc.NumRenderTargets = 1; // we are only binding one render target

										  // create the pso
			CheckSucceeded( d3d12->device->CreateGraphicsPipelineState(&psoDesc,IID_PPV_ARGS(&d3d12->pipelineStateObject)));
		}
		// Create vertex buffer

		
		/********************************************************************************************************************/
		// -- create default heap --
		{
			// default heap is memory on the GPU. Only the GPU has access to this memory
			// To get data into this heap, we will have to upload the data using
			// an upload heap
			d3d12->device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
				D3D12_HEAP_FLAG_NONE, // no flags
				&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
				D3D12_RESOURCE_STATE_COPY_DEST, // we will start this heap in the copy destination state since we will copy data
												// from the upload heap to this heap
				nullptr, // optimized clear value must be null for this type of resource. used for render targets and depth/stencil buffers
				IID_PPV_ARGS(&d3d12->vertexBuffer));

			// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
			d3d12->vertexBuffer->SetName(L"Vertex Buffer Resource Heap");

			// create upload heap
			// upload heaps are used to upload data to the GPU. CPU can write to it, GPU can read from it
			// We will upload the vertex buffer using this heap to the default heap
			ID3D12Resource* vBufferUploadHeap;
			d3d12->device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
				D3D12_HEAP_FLAG_NONE, // no flags
				&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
				D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
				nullptr,
				IID_PPV_ARGS(&vBufferUploadHeap));
			vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

			// store vertex buffer in upload heap
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast<BYTE*>(vList); // pointer to our vertex array
			vertexData.RowPitch = vBufferSize; // size of all our triangle vertex data
			vertexData.SlicePitch = vBufferSize; // also the size of our triangle vertex data

												 // we are now creating a command with the command list to copy the data from
												 // the upload heap to the default heap
			UpdateSubresources(d3d12->commandList,d3d12->vertexBuffer,vBufferUploadHeap,0,0,1,&vertexData);

			// transition the vertex buffer data from copy destination state to vertex buffer state
			d3d12->commandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(d3d12->vertexBuffer,D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));


			// create default heap to hold index buffer
			d3d12->device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
				D3D12_HEAP_FLAG_NONE, // no flags
				&CD3DX12_RESOURCE_DESC::Buffer(iBufferSize), // resource description for a buffer
				D3D12_RESOURCE_STATE_COPY_DEST, // start in the copy destination state
				nullptr, // optimized clear value must be null for this type of resource
				IID_PPV_ARGS(&d3d12->indexBuffer));

			// we can give resource heaps a name so when we debug with the graphics debugger we know what resource we are looking at
			d3d12->vertexBuffer->SetName(L"Index Buffer Resource Heap");

			// create upload heap to upload index buffer
			ID3D12Resource* iBufferUploadHeap;
			d3d12->device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
				D3D12_HEAP_FLAG_NONE, // no flags
				&CD3DX12_RESOURCE_DESC::Buffer(vBufferSize), // resource description for a buffer
				D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
				nullptr,
				IID_PPV_ARGS(&iBufferUploadHeap));
			vBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

			// store vertex buffer in upload heap
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = reinterpret_cast<BYTE*>(iList); // pointer to our index array
			indexData.RowPitch = iBufferSize; // size of all our index buffer
			indexData.SlicePitch = iBufferSize; // also the size of our index buffer

												// we are now creating a command with the command list to copy the data from
												// the upload heap to the default heap
			UpdateSubresources(d3d12->commandList,d3d12->indexBuffer,iBufferUploadHeap,0,0,1,&indexData);

			// transition the vertex buffer data from copy destination state to vertex buffer state
			d3d12->commandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(d3d12->indexBuffer,D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));


			// Now we execute the command list to upload the initial assets (triangle data)
			d3d12->commandList->Close();
			ID3D12CommandList* ppCommandLists[] = { d3d12->commandList };
			d3d12->commandQueue->ExecuteCommandLists(_countof(ppCommandLists),ppCommandLists);

			// increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing
			d3d12->fenceValue[d3d12->frameIndex]++;
			CheckSucceeded(d3d12->commandQueue->Signal(d3d12->fence[d3d12->frameIndex],d3d12->fenceValue[d3d12->frameIndex]));

			// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
			d3d12->vertexBufferView.BufferLocation = d3d12->vertexBuffer->GetGPUVirtualAddress();
			d3d12->vertexBufferView.StrideInBytes = sizeof(Vertex);
			d3d12->vertexBufferView.SizeInBytes = vBufferSize;

			// create a vertex buffer view for the triangle. We get the GPU memory address to the vertex pointer using the GetGPUVirtualAddress() method
			d3d12->indexBufferView.BufferLocation = d3d12->indexBuffer->GetGPUVirtualAddress();
			d3d12->indexBufferView.Format = DXGI_FORMAT_R32_UINT; // 32-bit unsigned integer (this is what a dword is, double word, a word is 2 bytes)
			d3d12->indexBufferView.SizeInBytes = iBufferSize;
		}

		{
			// Fill out the Viewport
			d3d12->viewport.TopLeftX = 0;
			d3d12->viewport.TopLeftY = 0;
			d3d12->viewport.Width = d3d12->Width;
			d3d12->viewport.Height = d3d12->Height;
			d3d12->viewport.MinDepth = 0.0f;
			d3d12->viewport.MaxDepth = 1.0f;

			// Fill out a scissor rect
			d3d12->scissorRect.left = 0;
			d3d12->scissorRect.top = 0;
			d3d12->scissorRect.right = d3d12->Width;
			d3d12->scissorRect.bottom = d3d12->Height;
		}
	}

	void d3d12EnableDebugLayer(D3D12_Window &d3d12){
		CheckSucceeded(D3D12GetDebugInterface(IID_PPV_ARGS(&d3d12->debugInterface)));
		d3d12->debugInterface->EnableDebugLayer();
	}

	void d3d12WaitForPreviousFrame(D3D12_Window &d3d12) {
		// swap the current rtv buffer index so we draw on the correct buffer
		d3d12->frameIndex = d3d12->swapChain->GetCurrentBackBufferIndex();

		// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
		// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
		if(d3d12->fence[d3d12->frameIndex]->GetCompletedValue() < d3d12->fenceValue[d3d12->frameIndex]) {
			// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
			CheckSucceeded( d3d12->fence[d3d12->frameIndex]->SetEventOnCompletion(d3d12->fenceValue[d3d12->frameIndex],d3d12->fenceEvent));

			// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
			// has reached "fenceValue", we know the command queue has finished executing
			WaitForSingleObject(d3d12->fenceEvent,INFINITE);
		}

		// increment fenceValue for next frame
		d3d12->fenceValue[d3d12->frameIndex]++;
	}

	void d3d12UpdatePipeLine(D3D12_Window &d3d12,float clear_color[4]){

		// We have to wait for the GPU to finish with the command allocator before we reset it
		d3d12WaitForPreviousFrame(d3d12);

		// we can only reset an allocator once the GPU is done with it
		// resetting an allocator frees the memory that the command list was stored in
		CheckSucceeded(d3d12->commandAllocator[d3d12->frameIndex]->Reset());

		// reset the command list. by resetting the command list we are putting it into
		// a recording state so we can start recording commands into the command allocator.
		// the command allocator that we reference here may have multiple command lists
		// associated with it, but only one can be recording at any time. Make sure
		// that any other command lists associated to this command allocator are in
		// the closed state (not recording).
		// Here you will pass an initial pipeline state object as the second parameter,
		// but in this tutorial we are only clearing the rtv, and do not actually need
		// anything but an initial default pipeline, which is what we get by setting
		// the second parameter to NULL
		CheckSucceeded(d3d12->commandList->Reset(d3d12->commandAllocator[d3d12->frameIndex],d3d12->pipelineStateObject));

		// here we start recording commands into the commandList (which all the commands will be stored in the commandAllocator)

		// transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
		d3d12->commandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(d3d12->renderTargets[d3d12->frameIndex],D3D12_RESOURCE_STATE_PRESENT,D3D12_RESOURCE_STATE_RENDER_TARGET));

		// here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(d3d12->rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),d3d12->frameIndex,d3d12->rtvDescriptorSize);

		// set the render target for the output merger stage (the output of the pipeline)
		d3d12->commandList->OMSetRenderTargets(1,&rtvHandle,FALSE,nullptr);

		// Clear the render target by using the ClearRenderTargetView command
		d3d12->commandList->ClearRenderTargetView(rtvHandle,clear_color,0,nullptr);

		// draw triangle
		d3d12->commandList->SetGraphicsRootSignature(d3d12->rootSignature); // set the root signature
		d3d12->commandList->RSSetViewports(1,&d3d12->viewport); // set the view ports
		d3d12->commandList->RSSetScissorRects(1,&d3d12->scissorRect); // set the scissor rects
		d3d12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology
		d3d12->commandList->IASetVertexBuffers(0,1,&d3d12->vertexBufferView); // set the vertex buffer (using the vertex buffer view)
		d3d12->commandList->IASetIndexBuffer(&d3d12->indexBufferView);
		d3d12->commandList->DrawIndexedInstanced(6,1,0,0,0); // finally draw 3 vertices (draw the triangle)

											 // transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
											 // warning if present is called on the render target when it's not in the present state
		d3d12->commandList->ResourceBarrier(1,&CD3DX12_RESOURCE_BARRIER::Transition(d3d12->renderTargets[d3d12->frameIndex],D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PRESENT));

		CheckSucceeded(d3d12->commandList->Close());
	}

	// Dll Functions
	//------------------------------------------------------------------------------------------------
	const char * __stdcall dll_info() {
		return "BPD_D3D12";
	}

	void _stdcall dll_release(D3D12_Window& d3d12) {
		// wait for the GPU to finish all frames
		for(int i = 0; i < d3d12->FRAME; ++i) {
			d3d12->frameIndex = i;
			d3d12WaitForPreviousFrame(d3d12);
		}

		// get swapchain out of full screen before exiting
		BOOL fs = false;
		if(d3d12->swapChain->GetFullscreenState(&fs,NULL))
			d3d12->swapChain->SetFullscreenState(false,NULL);

		SAFE_RELEASE(d3d12->device);
		SAFE_RELEASE(d3d12->swapChain);
		SAFE_RELEASE(d3d12->commandQueue);
		SAFE_RELEASE(d3d12->rtvDescriptorHeap);
		SAFE_RELEASE(d3d12->commandList);

		for(int i = 0; i < d3d12->FRAME; ++i) {
			SAFE_RELEASE(d3d12->renderTargets[i]);
			SAFE_RELEASE(d3d12->commandAllocator[i]);
			SAFE_RELEASE(d3d12->fence[i]);
		};

		SAFE_RELEASE(d3d12->pipelineStateObject);
		SAFE_RELEASE(d3d12->rootSignature);
		SAFE_RELEASE(d3d12->vertexBuffer);
		SAFE_RELEASE(d3d12->indexBuffer);
	}

	int __stdcall dll_createGraphics(D3D12_Window& d3d12,HWND& hWnd) {
		d3d12 = new _D3D12_Window;

		// a triangle
		Vertex vList[] = {
			{ DirectX::XMFLOAT3{ -0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT4{1.0, 0.0, 0.0, 1.0} }, // top left
			{ DirectX::XMFLOAT3{ 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT4{ 0.0, 1.0, 0.0, 1.0 } }, // top right
			{ DirectX::XMFLOAT3{ -0.5f, -0.5f, 0.5f }, DirectX::XMFLOAT4{ 0.0, 0.0, 1.0, 1.0 } }, // bottom left
			{ DirectX::XMFLOAT3{ 0.5f, -0.5f, 0.5f }, DirectX::XMFLOAT4{ 0.0, 0.0, 0.0, 1.0 } }, // botom right
		};

		DWORD iList[] = {
			0, 1, 2, // first triangle
			2, 1, 3, // second triangle
		};

		d3d12EnableDebugLayer(d3d12);

		InitD3D(d3d12, hWnd, vList, 4, iList, 6);

		/*

		d3d12CreateFactory(d3d12);

		d3d12CreateAdapters(d3d12);

		d3d12CreateCommandQueue(d3d12);

		d3d12CreateSwapChain(d3d12, hWnd, false);

		d3d12CreateBackBuffers(d3d12);

		d3d12CreateCommandAllocator(d3d12);

		d3d12CreateCommandList(d3d12);

		d3d12CreateFence(d3d12);

		d3d12CreateRootSignature(d3d12);

		d3d12CreateVertexShader(d3d12, L"VertexShader.hlsl", "main");

		d3d12CreatePixelShader(d3d12, L"PixelShader.hlsl", "main");

		d3d12CreateInputLayout(d3d12);

		d3d12CreatePipeline(d3d12);

		d3d12Heap(d3d12, vList, 3);

		d3d12CreateViewPort(d3d12);*/

		return 0;
	}

	void __stdcall dll_update(D3D12_Window &d3d12) {
		static uint64_t frameCounter = 0;
		static double elapsedSeconds = 0.0;
		static std::chrono::high_resolution_clock clock;
		static auto t0 = clock.now();

		frameCounter++;
		auto t1 = clock.now();
		auto deltaTime = t1 - t0;
		t0 = t1;

		elapsedSeconds += deltaTime.count() * 1e-9;
		if(elapsedSeconds > 1.0) {
			auto fps = frameCounter / elapsedSeconds;
			printf("FPS: %f\n", fps);

			frameCounter = 0;
			elapsedSeconds = 0.0;
		}

	}

	void __stdcall dll_draw(D3D12_Window &d3d12,Buffer buffer[], int buffer_count,float clear_color[4]) {

		d3d12UpdatePipeLine(d3d12, clear_color); // update the pipeline by sending commands to the command queue

						  // create an array of command lists (only one command list here)
		ID3D12CommandList* ppCommandLists[] = { d3d12->commandList };

		// execute the array of command lists
		d3d12->commandQueue->ExecuteCommandLists(_countof(ppCommandLists),ppCommandLists);

		// this command goes in at the end of our command queue. we will know when our command queue 
		// has finished because the fence value will be set to "fenceValue" from the GPU since the command
		// queue is being executed on the GPU
		CheckSucceeded(d3d12->commandQueue->Signal(d3d12->fence[d3d12->frameIndex],d3d12->fenceValue[d3d12->frameIndex]));

		// present the current back buffer
		CheckSucceeded(d3d12->swapChain->Present(0,0));

	}

#ifdef __cplusplus
}
#endif