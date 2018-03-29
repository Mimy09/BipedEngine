#pragma once

namespace bpd{
	struct _D3D11_Window {
		IDXGISwapChain* swapchain;
		ID3D11Device* device;
		ID3D11DeviceContext* device_context;
		ID3D11Texture2D *pBackBuffer;
		ID3D11RenderTargetView *rtv;
		ID3D11InputLayout *vertexLayout;
		D3D11_VIEWPORT vp;
		ID3D11Buffer *vertexBuffer;
		ID3D11Buffer *indexBuffer;

		ID3D11VertexShader *vertexShader;
		ID3D11PixelShader *pixelShader;

		ID3D11Buffer* ConstantBuffers = NULL;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};
}