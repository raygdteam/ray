#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "../../Engine/ishader.hpp"

namespace ray::renderer::directx11
{

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
		ID3D11PixelShader* m_PixelShader;
		ID3D11InputLayout* m_Layout;
		ID3D11Buffer* m_MatrixBuffer;

	};

}



