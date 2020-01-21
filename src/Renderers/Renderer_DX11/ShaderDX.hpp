#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "../../Engine/ishader.hpp"

namespace ray::renderer::directx11
{

	enum class eShaderTypeDX
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel
	};

	class ShaderDX :
		public IShader
	{
	public:
		ShaderDX();
		~ShaderDX() override;

		void Compile() override {} //not using
		void SetSources(std::string&, std::string&) override {} //not using
		void SetSources(pcstr, pcstr) override {} //not using
		void UseShader() override;
		void Destroy() override;
		HRESULT Compile(const WCHAR*, pcstr, pcstr, ID3DBlob**, ID3DBlob*);
		void Create(eShaderTypeDX, const WCHAR*);

		void SetParams(size_t, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
		void SetInputElement(D3D11_INPUT_ELEMENT_DESC*, size_t);

	private:
		struct MatrixBufferType
		{
			DirectX::XMMATRIX World;
			DirectX::XMMATRIX View;
			DirectX::XMMATRIX Projection;
		};

	private:
		ID3D11VertexShader* m_VertexShader;
		ID3D11HullShader* m_HullShader;
		ID3D11DomainShader* m_DomainShader;
		ID3D11GeometryShader* m_GeometryShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11InputLayout* m_Layout;
		ID3D11Buffer* m_MatrixBuffer;
		ID3DBlob* m_VertexShaderBuffer;
		size_t m_IndexCount;

	};

}



