#include "bpd_d3d11.h"
#include "bpd-d3d11-def.h"

#define Assert(x) if (!(x)) { MessageBoxA(0, #x, "Assertion Failed", MB_OK); __debugbreak(); }
#define Check(x) if (!(x)) { MessageBoxA(0, #x, "Check Failed", MB_OK); __debugbreak(); }
#define CheckSucceeded(hresult) Check(SUCCEEDED(hresult))

using namespace bpd;

#ifdef __cplusplus
extern "C" {
#endif
	/** Private Fuctions **************************************************************/

	void PrintCompileErrorMessages(ID3DBlob* errors) {
		char temp[1024];
		MessageBox(0, "Compile Shader Error", "Error", MB_OK | MB_ICONERROR);
		//sprintf_s(temp,sizeof(temp),"%.*s",(int)errors->GetBufferSize(),(char *)errors->GetBufferPointer());
		OutputDebugStringA(temp);
	}

	HRESULT CompileShader(const WCHAR *filename,const char *entrypoint,const char *profile,ID3DBlob **out_code) {
		ID3DBlob *errors;
		HRESULT hresult = D3DCompileFromFile(filename,0,0,entrypoint,profile,D3DCOMPILE_DEBUG,0,out_code,&errors);
		if(hresult != S_OK) {
			PrintCompileErrorMessages(errors);
		}
		return hresult;
	}

	void d3d11SwapChain(D3D11_Window & d3d11,HWND& hWnd) {
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = _countof(featureLevels);

		RECT rc;
		GetClientRect(hWnd,&rc);

		DXGI_SWAP_CHAIN_DESC dxgi_swapchain_desc = { 0 };
		dxgi_swapchain_desc.BufferCount = 1;
		dxgi_swapchain_desc.BufferDesc.Width = rc.right - rc.left;
		dxgi_swapchain_desc.BufferDesc.Height = rc.bottom - rc.top;
		dxgi_swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgi_swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
		dxgi_swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
		dxgi_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgi_swapchain_desc.OutputWindow = hWnd;
		dxgi_swapchain_desc.SampleDesc.Count = 1;
		dxgi_swapchain_desc.SampleDesc.Quality = 0;
		dxgi_swapchain_desc.Windowed = TRUE;

		CheckSucceeded(D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,NULL,featureLevels,numFeatureLevels,D3D11_SDK_VERSION,&dxgi_swapchain_desc,&d3d11->swapchain,&d3d11->device,NULL,&d3d11->device_context));
	}
	void d3d11RenderTargetView(D3D11_Window & d3d11) {
		CheckSucceeded(d3d11->swapchain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&d3d11->pBackBuffer));
		HRESULT hr = d3d11->device->CreateRenderTargetView(d3d11->pBackBuffer,NULL,&d3d11->rtv);
		d3d11->pBackBuffer->Release();
		CheckSucceeded(hr);
		d3d11->device_context->OMSetRenderTargets(1,&d3d11->rtv,NULL);
	}
	void d3d11ViewPort(D3D11_Window & d3d11,HWND& hWnd) {
		RECT rc;
		GetClientRect(hWnd,&rc);

		d3d11->vp.Width = (FLOAT)(rc.right - rc.left);
		d3d11->vp.Height = (FLOAT)(rc.bottom - rc.top);
		d3d11->vp.MinDepth = 0.0f;
		d3d11->vp.MaxDepth = 1.0f;
		d3d11->vp.TopLeftX = 0;
		d3d11->vp.TopLeftY = 0;
		d3d11->device_context->RSSetViewports(1,&d3d11->vp);
	}
	void d3d11Buffer(D3D11_Window & d3d11, Vertex vertices[],int vertex_size,WORD indices[],int index_size) {
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA InitData;

		ZeroMemory(&bd,sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * index_size;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = indices;
		CheckSucceeded(d3d11->device->CreateBuffer(&bd,&InitData,&d3d11->indexBuffer));

		ZeroMemory(&bd,sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Vertex) * vertex_size;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		ZeroMemory(&InitData,sizeof(InitData));
		InitData.pSysMem = vertices;
		CheckSucceeded(d3d11->device->CreateBuffer(&bd,&InitData,&d3d11->vertexBuffer));
	}
	void d3d11InitDeviceObjects(D3D11_Window & d3d11) {
		ID3DBlob *VSBlob;
		ID3DBlob *PSBlob;

		CheckSucceeded(CompileShader(L"BasicShader.hlsl","vs_main","vs_4_0",&VSBlob));
		CheckSucceeded(CompileShader(L"BasicShader.hlsl","ps_main","ps_4_0",&PSBlob));

		CheckSucceeded(d3d11->device->CreateVertexShader(VSBlob->GetBufferPointer(),VSBlob->GetBufferSize(),NULL,&d3d11->vertexShader));
		CheckSucceeded(d3d11->device->CreatePixelShader(PSBlob->GetBufferPointer(),PSBlob->GetBufferSize(),NULL,&d3d11->pixelShader));
		PSBlob->Release();

		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numElements = ARRAYSIZE(layout);
		CheckSucceeded(d3d11->device->CreateInputLayout(layout,numElements,VSBlob->GetBufferPointer(),VSBlob->GetBufferSize(),&d3d11->vertexLayout));
		VSBlob->Release();
	}
	void d3d11DeviceContext(D3D11_Window & d3d11) {
		d3d11->device_context->IASetInputLayout(d3d11->vertexLayout);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		d3d11->device_context->IASetIndexBuffer(d3d11->indexBuffer,DXGI_FORMAT_R16_UINT,0);
		d3d11->device_context->IASetVertexBuffers(0,1,&d3d11->vertexBuffer,&stride,&offset);
		d3d11->device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		d3d11->device_context->VSSetShader(d3d11->vertexShader,NULL,0);
		d3d11->device_context->PSSetShader(d3d11->pixelShader,NULL,0);

		d3d11->vertexBuffer->Release();
		d3d11->indexBuffer->Release();
	}

	/** Dll Functions **************************************************************/

	const char * __stdcall dll_info() {
		return "BPD_D3D11";
	}

	void _stdcall dll_release(D3D11_Window& d3d11){
		d3d11->swapchain->Release();
		d3d11->device->Release();
		d3d11->vertexShader->Release();
		d3d11->pixelShader->Release();
	}

	int __stdcall dll_createGraphics(D3D11_Window& d3d11, HWND& hWnd) {

		d3d11 = new _D3D11_Window;

		d3d11SwapChain(d3d11, hWnd);
		d3d11RenderTargetView(d3d11);
		d3d11InitDeviceObjects(d3d11);
		d3d11ViewPort(d3d11,hWnd);

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
		for (int i = 0; i < buffer_count; i++){
			d3d11Buffer(d3d11,buffer[i].vertex,buffer[i].vertex_count,buffer[i].index,buffer[i].index_count);
			d3d11DeviceContext(d3d11);

			d3d11->device_context->DrawIndexed(buffer[i].index_count,0,0);
		}

		d3d11->swapchain->Present(0,0);
		d3d11->device_context->ClearRenderTargetView(d3d11->rtv,clear_color);
	}



#ifdef __cplusplus
}
#endif