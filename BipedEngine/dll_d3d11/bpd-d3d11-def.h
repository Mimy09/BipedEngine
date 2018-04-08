#pragma once

namespace bpd{

	struct _D3D11_Window {
		/*IDXGISwapChain* swapchain;
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

		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;

		ID3D11Buffer* cbPerObjectBuffer;

		INT Width;
		INT Height;

		DirectX::XMMATRIX WVP;
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX camView;
		DirectX::XMMATRIX camProjection;

		DirectX::XMFLOAT4 camPosition;
		DirectX::XMFLOAT4 camTarget;
		DirectX::XMFLOAT4 camUp;

		


		DirectX::XMMATRIX cube1World;
		DirectX::XMMATRIX cube2World;
		DirectX::XMMATRIX Rotation;
		DirectX::XMMATRIX Scale;
		DirectX::XMMATRIX Translation;
		float rot = 0.01f;*/

		IDXGISwapChain* SwapChain;
		ID3D11Device* d3d11Device;
		ID3D11DeviceContext* d3d11DevCon;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Buffer* squareIndexBuffer;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11Texture2D* depthStencilBuffer;
		ID3D11Buffer* squareVertBuffer;
		ID3D11VertexShader* VS;
		ID3D11PixelShader* PS;
		ID3DBlob* VS_Buffer;
		ID3DBlob* PS_Buffer;
		ID3D11InputLayout* vertLayout;
		ID3D11Buffer* cbPerObjectBuffer;

		ConstantBufferPerObject cbPerObj;

		INT Width;
		INT Height;

		DirectX::XMMATRIX WVP;


		std::vector<Transform> transform_list;

		/*DirectX::XMMATRIX cube1World;
		DirectX::XMMATRIX cube2World;*/

		DirectX::XMMATRIX camView;
		DirectX::XMMATRIX camProjection;

		DirectX::XMFLOAT4 camPosition;
		DirectX::XMFLOAT4 camTarget;
		DirectX::XMFLOAT4 camUp;

		/*DirectX::XMMATRIX Rotation;
		DirectX::XMMATRIX Scale;
		DirectX::XMMATRIX Translation;*/


		float rot = 0.01f;
		


	};
}