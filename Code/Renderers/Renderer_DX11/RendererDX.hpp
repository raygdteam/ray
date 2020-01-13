#pragma once
#include "../../Engine/irenderer.hpp"

#include <d3d11.h>
#include <DirectXMath.h>

#include "ShaderDX.hpp"

namespace ray::renderer::directx11
{

	class RendererDX :
		public ray::renderer::IRenderer
	{
	public:
		void WindowSizeChanged(u32 width, u32 height) override {}
		RendererDX();
		~RendererDX() override {}

		bool Init() override;
		void Draw() override;
		void Destroy() override;
		void BeginFrame() override;
		void EndFrame() override;

	private:
		D3D_DRIVER_TYPE m_DriverType;
		D3D_FEATURE_LEVEL m_FeatureLevel;
		/*ID3D11Device* m_Device;
		ID3D11DeviceContext* m_Context;
		IDXGISwapChain* m_SwapChain;
		ID3D11RenderTargetView* m_RenderTargetView;*/
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilState* m_DepthStencilState;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11RasterizerState* m_RasterState;
		u32 m_VideoCardMemory;
		char m_VideoCardDescr[128];

		bool m_VSyncEnabled;
		bool m_Fullscreen;

		DirectX::XMMATRIX m_ProjectionMatrix;
		DirectX::XMMATRIX m_OrthoMatrix;
		DirectX::XMMATRIX m_WorldMatrix;

	};

}

}

