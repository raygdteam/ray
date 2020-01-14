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

		void Compile() override;
		void SetSources(std::string&, std::string&) override;
		void SetSources(pcstr, pcstr) override;
		void UseShader() override;
		void Destroy() override;
		HRESULT Compile(pcstr, pcstr, pcstr, ID3DBlob*, ID3DBlob*);
		void Create(eShaderTypeDX, pcstr);

		void SetMatrixes(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
	private:
		struct MatrixBufferType
		{
			DirectX::XMMATRIX World;
			DirectX::XMMATRIX View;
			DirectX::XMMATRIX Projection;
		};

	private:
		void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	private:
		ID3D11VertexShader* m_VertexShader;
		ID3D11HullShader* m_HullShader;
		ID3D11DomainShader* m_DomainShader;
		ID3D11GeometryShader* m_GeometryShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11InputLayout* m_Layout;
		ID3D11Buffer* m_MatrixBuffer;

	};

}



