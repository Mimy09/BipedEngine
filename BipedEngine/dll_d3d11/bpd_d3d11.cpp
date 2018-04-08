#include "bpd_d3d11.h"
#include "bpd-d3d11-def.h"

#define Assert(x) if (!(x)) { MessageBoxA(0, #x, "Assertion Failed", MB_OK); __debugbreak(); }
#define Check(x) if (!(x)) { MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak(); }
#define CheckSucceeded(hresult) Check(SUCCEEDED(hresult))

using namespace bpd;
using namespace DirectX;

#ifdef __cplusplus
extern "C" {
#endif
	/** Private Fuctions **************************************************************/

	void PrintCompileErrorMessages(ID3DBlob* errors) {
		MessageBox(0,(char *)errors->GetBufferPointer(), "Error", MB_OK | MB_ICONERROR);
		//sprintf_s(temp,sizeof(temp),"%.*s",(int)errors->GetBufferSize(),(char *)errors->GetBufferPointer());
	}

	HRESULT CompileShader(const WCHAR *filename,const char *entrypoint,const char *profile,ID3DBlob **out_code) {
		ID3DBlob *errors;
		HRESULT hresult = D3DCompileFromFile(filename,0,0,entrypoint,profile,D3DCOMPILE_DEBUG,0,out_code,&errors);
		if(hresult != S_OK) {
			PrintCompileErrorMessages(errors);
		}
		return hresult;
	}
	/*
	//void d3d11SwapChain(D3D11_Window & d3d11, HWND hWnd) {
	//	D3D_FEATURE_LEVEL featureLevels[] =
	//	{
	//		D3D_FEATURE_LEVEL_11_0,
	//		D3D_FEATURE_LEVEL_10_1,
	//		D3D_FEATURE_LEVEL_10_0,
	//	};
	//	UINT numFeatureLevels = _countof(featureLevels);

	//	DXGI_SWAP_CHAIN_DESC dxgi_swapchain_desc = { 0 };
	//	dxgi_swapchain_desc.BufferCount = 1;
	//	dxgi_swapchain_desc.BufferDesc.Width = d3d11->Width;
	//	dxgi_swapchain_desc.BufferDesc.Height = d3d11->Height;
	//	dxgi_swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	dxgi_swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	//	dxgi_swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	//	dxgi_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//	dxgi_swapchain_desc.OutputWindow = hWnd;
	//	dxgi_swapchain_desc.SampleDesc.Count = 1;
	//	dxgi_swapchain_desc.SampleDesc.Quality = 0;
	//	dxgi_swapchain_desc.Windowed = TRUE;

	//	CheckSucceeded(D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,featureLevels,numFeatureLevels,D3D11_SDK_VERSION,&dxgi_swapchain_desc,&d3d11->swapchain,&d3d11->device,NULL,&d3d11->device_context));
	//}
	//void d3d11RenderTargetView(D3D11_Window & d3d11) {
	//	CheckSucceeded(d3d11->swapchain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&d3d11->pBackBuffer));
	//	HRESULT hr = d3d11->device->CreateRenderTargetView(d3d11->pBackBuffer,NULL,&d3d11->rtv);
	//	d3d11->pBackBuffer->Release();
	//	CheckSucceeded(hr);
	//	d3d11->device_context->OMSetRenderTargets(1,&d3d11->rtv,NULL);
	//}
	//void d3d11ViewPort(D3D11_Window & d3d11) {
	//	d3d11->vp.Width = (FLOAT)(d3d11->Width);
	//	d3d11->vp.Height = (FLOAT)(d3d11->Height);
	//	d3d11->vp.MinDepth = 0.0f;
	//	d3d11->vp.MaxDepth = 1.0f;
	//	d3d11->vp.TopLeftX = 0;
	//	d3d11->vp.TopLeftY = 0;

	//	d3d11->device_context->RSSetViewports(1,&d3d11->vp);
	//}
	//void d3d11Buffer(D3D11_Window & d3d11, Vertex vertices[],int vertex_size,DWORD indices[],int index_size) {
	//	D3D11_BUFFER_DESC bd;
	//	D3D11_SUBRESOURCE_DATA InitData;

	//	ZeroMemory(&bd,sizeof(bd));
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(DWORD) * index_size;
	//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	bd.CPUAccessFlags = 0;
	//	bd.MiscFlags = 0;
	//	InitData.pSysMem = indices;
	//	CheckSucceeded(d3d11->device->CreateBuffer(&bd,&InitData,&d3d11->indexBuffer));

	//	ZeroMemory(&bd,sizeof(bd));
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(Vertex) * vertex_size;
	//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//	bd.CPUAccessFlags = 0;
	//	bd.MiscFlags = 0;
	//	ZeroMemory(&InitData,sizeof(InitData));
	//	InitData.pSysMem = vertices;
	//	CheckSucceeded(d3d11->device->CreateBuffer(&bd,&InitData,&d3d11->vertexBuffer));

	//	ZeroMemory(&bd,sizeof(bd));

	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(ConstantBufferPerObject);
	//	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	bd.CPUAccessFlags = 0;
	//	bd.MiscFlags = 0;
	//	CheckSucceeded(d3d11->device->CreateBuffer(&bd,NULL,&d3d11->cbPerObjectBuffer));
	//}
	//void d3d11InitDeviceObjects(D3D11_Window & d3d11) {
	//	ID3DBlob *VSBlob;
	//	ID3DBlob *PSBlob;

	//	CheckSucceeded(CompileShader(L"VertexShader.hlsl","main","vs_4_0",&VSBlob));
	//	CheckSucceeded(CompileShader(L"PixelShader.hlsl","main","ps_4_0",&PSBlob));

	//	CheckSucceeded(d3d11->device->CreateVertexShader(VSBlob->GetBufferPointer(),VSBlob->GetBufferSize(),NULL,&d3d11->vertexShader));
	//	CheckSucceeded(d3d11->device->CreatePixelShader(PSBlob->GetBufferPointer(),PSBlob->GetBufferSize(),NULL,&d3d11->pixelShader));
	//	PSBlob->Release();

	//	D3D11_INPUT_ELEMENT_DESC layout[] = {
	//		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	};

	//	UINT numElements = ARRAYSIZE(layout);
	//	CheckSucceeded(d3d11->device->CreateInputLayout(layout,numElements,VSBlob->GetBufferPointer(),VSBlob->GetBufferSize(),&d3d11->vertexLayout));
	//	VSBlob->Release();
	//}
	//void d3d11DepthBuffer(D3D11_Window & d3d11){

	//	//Describe our Depth/Stencil Buffer
	//	D3D11_TEXTURE2D_DESC depthStencilDesc;

	//	depthStencilDesc.Width = d3d11->Width;
	//	depthStencilDesc.Height = d3d11->Height;
	//	depthStencilDesc.MipLevels = 1;
	//	depthStencilDesc.ArraySize = 1;
	//	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//	depthStencilDesc.SampleDesc.Count = 1;
	//	depthStencilDesc.SampleDesc.Quality = 0;
	//	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	//	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//	depthStencilDesc.CPUAccessFlags = 0;
	//	depthStencilDesc.MiscFlags = 0;

	//	//Create the Depth/Stencil View
	//	d3d11->device->CreateTexture2D(&depthStencilDesc,NULL,&d3d11->depthStencilBuffer);
	//	d3d11->device->CreateDepthStencilView(d3d11->depthStencilBuffer,NULL,&d3d11->depthStencilView);

	//	d3d11->device_context->OMSetRenderTargets(1,&d3d11->rtv,d3d11->depthStencilView);
	//}

	//

	//void d3d11DeviceContext(D3D11_Window & d3d11) {
	//	d3d11->device_context->IASetInputLayout(d3d11->vertexLayout);

	//	UINT stride = sizeof(Vertex);
	//	UINT offset = 0;
	//	d3d11->device_context->IASetIndexBuffer(d3d11->indexBuffer,DXGI_FORMAT_R16_UINT,0);
	//	d3d11->device_context->IASetVertexBuffers(0,1,&d3d11->vertexBuffer,&stride,&offset);
	//	d3d11->device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	d3d11->device_context->VSSetShader(d3d11->vertexShader,NULL,0);
	//	d3d11->device_context->PSSetShader(d3d11->pixelShader,NULL,0);

	//	d3d11->vertexBuffer->Release();
	//	d3d11->indexBuffer->Release();
	//}
*/

	void d3d11InitD3D(D3D11_Window & d3d11,HWND hwnd){
		/********************************************************************************************************************/
		// -- Create the SwapChain -- //
		{
			//Describe our SwapChain Buffer
			DXGI_MODE_DESC bufferDesc;

			ZeroMemory(&bufferDesc,sizeof(DXGI_MODE_DESC));

			bufferDesc.Width = d3d11->Width;
			bufferDesc.Height = d3d11->Height;
			bufferDesc.RefreshRate.Numerator = 60;
			bufferDesc.RefreshRate.Denominator = 1;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			//Describe our SwapChain
			DXGI_SWAP_CHAIN_DESC swapChainDesc;

			ZeroMemory(&swapChainDesc,sizeof(DXGI_SWAP_CHAIN_DESC));

			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = hwnd;
			swapChainDesc.Windowed = TRUE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


			//Create our SwapChain
			CheckSucceeded( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,NULL,NULL,
				D3D11_SDK_VERSION,&swapChainDesc,&d3d11->SwapChain,&d3d11->d3d11Device,NULL,&d3d11->d3d11DevCon));
		}

		/********************************************************************************************************************/
		// -- Create the BackBuffer and Render Target -- //
		{
			// Create the BackBuffer
			ID3D11Texture2D* BackBuffer;
			CheckSucceeded(d3d11->SwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&BackBuffer));

			//Create our Render Target
			CheckSucceeded(d3d11->d3d11Device->CreateRenderTargetView(BackBuffer,NULL,&d3d11->renderTargetView));
			BackBuffer->Release();
		}

		/********************************************************************************************************************/
		// -- Create the Depth/Stencil Buffer -- //
		{
			//Describe our Depth/Stencil Buffer
			D3D11_TEXTURE2D_DESC depthStencilDesc;

			depthStencilDesc.Width = d3d11->Width;
			depthStencilDesc.Height = d3d11->Height;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;

			//Create the Depth/Stencil View
			d3d11->d3d11Device->CreateTexture2D(&depthStencilDesc,NULL,&d3d11->depthStencilBuffer);
			d3d11->d3d11Device->CreateDepthStencilView(d3d11->depthStencilBuffer,NULL,&d3d11->depthStencilView);
		}

		/********************************************************************************************************************/
		// -- Set the Render Target -- //
		{
			//Set our Render Target
			d3d11->d3d11DevCon->OMSetRenderTargets(1,&d3d11->renderTargetView,d3d11->depthStencilView);
		}
	}

	void d3d11Init(D3D11_Window &d3d11){
		/********************************************************************************************************************/
		// -- Compile and create the shaders -- //
		{
			CheckSucceeded(CompileShader(L"VertexShader.hlsl","main","vs_4_0",&d3d11->VS_Buffer));
			CheckSucceeded(CompileShader(L"PixelShader.hlsl","main","ps_4_0",&d3d11->PS_Buffer));

			//Create the Shader Objects
			CheckSucceeded(d3d11->d3d11Device->CreateVertexShader(d3d11->VS_Buffer->GetBufferPointer(),d3d11->VS_Buffer->GetBufferSize(),NULL,&d3d11->VS));
			CheckSucceeded(d3d11->d3d11Device->CreatePixelShader(d3d11->PS_Buffer->GetBufferPointer(),d3d11->PS_Buffer->GetBufferSize(),NULL,&d3d11->PS));

			//Set Vertex and Pixel Shaders
			d3d11->d3d11DevCon->VSSetShader(d3d11->VS,0,0);
			d3d11->d3d11DevCon->PSSetShader(d3d11->PS,0,0);
		}
		
		/********************************************************************************************************************/
		// -- Create layout -- //
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			//Create the Input Layout
			CheckSucceeded(d3d11->d3d11Device->CreateInputLayout(layout,numElements,d3d11->VS_Buffer->GetBufferPointer(),
				d3d11->VS_Buffer->GetBufferSize(),&d3d11->vertLayout));

			//Set the Input Layout
			d3d11->d3d11DevCon->IASetInputLayout(d3d11->vertLayout);

			//Set Primitive Topology
			d3d11->d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		/********************************************************************************************************************/
		// -- Create Viewport -- //
		{
			//Create the Viewport
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport,sizeof(D3D11_VIEWPORT));

			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = d3d11->Width;
			viewport.Height = d3d11->Height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			//Set the Viewport
			d3d11->d3d11DevCon->RSSetViewports(1,&viewport);
		}

		/********************************************************************************************************************/
		// -- Create const buffer -- //
		{
			//Create the buffer to send to the cbuffer in effect file
			D3D11_BUFFER_DESC cbbd;
			ZeroMemory(&cbbd,sizeof(D3D11_BUFFER_DESC));

			cbbd.Usage = D3D11_USAGE_DEFAULT;
			cbbd.ByteWidth = sizeof(ConstantBufferPerObject);
			cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbbd.CPUAccessFlags = 0;
			cbbd.MiscFlags = 0;

			CheckSucceeded(d3d11->d3d11Device->CreateBuffer(&cbbd,NULL,&d3d11->cbPerObjectBuffer));
		}

	}

	/** Dll Functions **************************************************************/

	const char * __stdcall dll_info() {
		return "BPD_D3D11";
	}

	void __stdcall dll_reloadBuffers(bpd::D3D11_Window &d3d11, bpd::Buffer buffer[], int buffer_count){
		/********************************************************************************************************************/
		// -- Create index Buffer -- //
		{
			D3D11_BUFFER_DESC indexBufferDesc;
			ZeroMemory(&indexBufferDesc,sizeof(indexBufferDesc));

			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = sizeof(DWORD) * buffer[0].index_count;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA iinitData;

			iinitData.pSysMem = buffer[0].index;
			CheckSucceeded(d3d11->d3d11Device->CreateBuffer(&indexBufferDesc,&iinitData,&d3d11->squareIndexBuffer));

			d3d11->d3d11DevCon->IASetIndexBuffer(d3d11->squareIndexBuffer,DXGI_FORMAT_R32_UINT,0);
		}

		/********************************************************************************************************************/
		// -- Create vertex Buffer -- //
		{
			D3D11_BUFFER_DESC vertexBufferDesc;
			ZeroMemory(&vertexBufferDesc,sizeof(vertexBufferDesc));

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * buffer[0].vertex_count;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA vertexBufferData;

			ZeroMemory(&vertexBufferData,sizeof(vertexBufferData));
			vertexBufferData.pSysMem = buffer[0].vertex;
			CheckSucceeded(d3d11->d3d11Device->CreateBuffer(&vertexBufferDesc,&vertexBufferData,&d3d11->squareVertBuffer));

			//Set the vertex buffer
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			d3d11->d3d11DevCon->IASetVertexBuffers(0,1,&d3d11->squareVertBuffer,&stride,&offset);
		}

	}

	void _stdcall dll_release(D3D11_Window& d3d11){
		/*d3d11->swapchain->Release();
		d3d11->device->Release();
		d3d11->vertexShader->Release();
		d3d11->pixelShader->Release();
		d3d11->depthStencilView->Release();
		d3d11->depthStencilBuffer->Release();
		d3d11->cbPerObjectBuffer->Release();*/

		d3d11->SwapChain->Release();
		d3d11->d3d11Device->Release();
		d3d11->d3d11DevCon->Release();
		d3d11->renderTargetView->Release();
		d3d11->squareVertBuffer->Release();
		d3d11->squareIndexBuffer->Release();
		d3d11->VS->Release();
		d3d11->PS->Release();
		d3d11->VS_Buffer->Release();
		d3d11->PS_Buffer->Release();
		d3d11->vertLayout->Release();
		d3d11->depthStencilView->Release();
		d3d11->depthStencilBuffer->Release();
		d3d11->cbPerObjectBuffer->Release();
	}

	int __stdcall dll_createGraphics(D3D11_Window& d3d11, HWND& hWnd) {
		d3d11 = new _D3D11_Window;

		RECT rc;
		GetClientRect(hWnd,&rc);
		d3d11->Width = rc.right - rc.left;
		d3d11->Height = rc.bottom - rc.top;

		d3d11InitD3D(d3d11, hWnd);
		d3d11Init(d3d11);

		return 0;
	}

	void __stdcall dll_update(D3D11_Window &d3d11){
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
			char buffer[500];
			auto fps = frameCounter / elapsedSeconds;
			printf("FPS: %f\n",fps);

			frameCounter = 0;
			elapsedSeconds = 0.0;
		}
	}

	void __stdcall dll_draw(D3D11_Window &d3d11, Buffer buffer[],int buffer_count,float clear_color[4]){

		//Clear our back buffer
		d3d11->d3d11DevCon->ClearRenderTargetView(d3d11->renderTargetView,clear_color);

		//Refresh the Depth/Stencil view
		d3d11->d3d11DevCon->ClearDepthStencilView(d3d11->depthStencilView,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f,0);

		///////////////**************new**************////////////////////
		for (int i = 0; i < d3d11->transform_list.size(); i++){
			//Set the WVP matrix and send it to the constant buffer in effect file
			XMMATRIX tempMat = XMLoadFloat4x4(&d3d11->transform_list[i].World);

			d3d11->WVP = tempMat * d3d11->camView * d3d11->camProjection;
			XMMATRIX transposed = DirectX::XMMatrixTranspose(d3d11->WVP);
			XMStoreFloat4x4(&d3d11->cbPerObj.wvpMat,transposed);

			d3d11->d3d11DevCon->UpdateSubresource(d3d11->cbPerObjectBuffer,0,NULL,&d3d11->cbPerObj,0,0);
			d3d11->d3d11DevCon->VSSetConstantBuffers(0,1,&d3d11->cbPerObjectBuffer);

			//Draw the first cube
			d3d11->d3d11DevCon->DrawIndexed(buffer[i].index_count,0,0);
		}

		//Present the back buffer to the screen
		d3d11->SwapChain->Present(1,0);
	}


	void __stdcall dll_setView(bpd::D3D11_Window &d3d11, DirectX::XMFLOAT4 cam_pos, DirectX::XMFLOAT4 cam_target, float fov){
		d3d11->camPosition = cam_pos;
		d3d11->camTarget = cam_target;
		d3d11->camUp = DirectX::XMFLOAT4(0.0f,1.0f,0.0f,0.0f);

		DirectX::XMVECTOR cPos = DirectX::XMLoadFloat4(&d3d11->camPosition);
		DirectX::XMVECTOR cTarg = DirectX::XMLoadFloat4(&d3d11->camTarget);
		DirectX::XMVECTOR cUp = DirectX::XMLoadFloat4(&d3d11->camUp);

		d3d11->camView = DirectX::XMMatrixLookAtLH(cPos,cTarg,cUp);

		d3d11->camProjection = DirectX::XMMatrixPerspectiveFovLH(fov*(3.14f / 180.0f),(float)d3d11->Width / (float)d3d11->Height,0.1f,1000.0f);
	}

	int __stdcall dll_createObj(bpd::D3D11_Window &d3d11, DirectX::XMFLOAT4 pos){
		Transform t;
		t.Position = pos;
		DirectX::XMVECTOR posVec = DirectX::XMLoadFloat4(&t.Position);
		DirectX::XMMATRIX tmpMat = DirectX::XMMatrixTranslationFromVector(posVec);

		//Define cube1's world space matrix
		DirectX::XMStoreFloat4x4(&t.Rotation,DirectX::XMMatrixIdentity());

		DirectX::XMStoreFloat4x4(&t.World,tmpMat); // store world matrix

		d3d11->transform_list.push_back(t);

		return d3d11->transform_list.size() - 1;
	}

	void __stdcall dll_transformObj( bpd::D3D11_Window &d3d11, int ID, DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT4 scale ){

		// now do cube2's world matrix
		// create rotation matrices for cube2
		XMMATRIX rotXMat = XMMatrixRotationX(rotation.x);
		XMMATRIX rotYMat = XMMatrixRotationY(rotation.y);
		XMMATRIX rotZMat = XMMatrixRotationZ(rotation.z);

		// add rotation to cube2's rotation matrix and store it
		XMMATRIX rotMat = rotZMat * (XMLoadFloat4x4(&d3d11->transform_list[ID].Rotation) * (rotXMat * rotYMat));
		XMStoreFloat4x4(&d3d11->transform_list[ID].Rotation,rotMat);

		// create translation matrix for cube 2 to offset it from cube 1 (its position relative to cube1
		XMMATRIX translationOffsetMat = XMMatrixTranslationFromVector(XMLoadFloat4(&position));

		// scale the cube
		XMMATRIX scaleMat = XMMatrixScaling(scale.x,scale.y,scale.z);

		// reuse worldMat. 
		// first we scale cube2. scaling happens relative to point 0,0,0, so you will almost always want to scale first
		// then we translate it. 
		// then we rotate it. rotation always rotates around point 0,0,0
		// finally we move it to cube 1's position, which will cause it to rotate around cube 1
		XMMATRIX worldMat = scaleMat * translationOffsetMat * rotMat;

		XMMATRIX wvpMat = XMLoadFloat4x4(&d3d11->transform_list[ID].World) * d3d11->camView * d3d11->camProjection; // create WVP matrix
		XMMATRIX transposed = XMMatrixTranspose(wvpMat); // must transpose WVP matrix for the GPU
		XMStoreFloat4x4(&d3d11->cbPerObj.wvpMat,transposed); // store transposed WVP matrix in constant buffer

		// store cube2's world matrix
		XMStoreFloat4x4(&d3d11->transform_list[ID].World,worldMat);

		/*d3d11->rot += .0005f;
		if(d3d11->rot > 6.26f)
			d3d11->rot = 0.0f;


		//Reset cube1World
		d3d11->cube1World = DirectX::XMMatrixIdentity();

		//Define cube1's world space matrix
		DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f);
		d3d11->Rotation = DirectX::XMMatrixRotationAxis(rotaxis,d3d11->rot);
		d3d11->Translation = DirectX::XMMatrixTranslation(0.0f,0.0f,4.0f);

		//Set cube1's world space using the transformations
		d3d11->cube1World = d3d11->Translation * d3d11->Rotation;

		//Reset cube2World
		d3d11->cube2World = DirectX::XMMatrixIdentity();

		//Define cube2's world space matrix
		d3d11->Rotation = DirectX::XMMatrixRotationAxis(rotaxis,d3d11->rot);
		d3d11->Scale = DirectX::XMMatrixScaling(1.3f,1.3f,1.3f);

		//Set cube2's world space matrix
		d3d11->cube2World = d3d11->Rotation * d3d11->Scale;*/
	}

#ifdef __cplusplus
}
#endif