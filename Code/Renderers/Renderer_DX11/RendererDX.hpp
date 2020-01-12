#pragma once
#include "../../Engine/irenderer.hpp"

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

#include "ShaderDX.hpp"

class RendererDX :
	public ray::renderer::IRenderer
{
public:
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
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;
	IDXGISwapChain* m_SwapChain;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11Buffer* m_VertexBuffer;
	ShaderDX shader;

};

