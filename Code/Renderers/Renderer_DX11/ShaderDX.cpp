#include "pch.hpp"
#include "ShaderDX.hpp"
#include <Core/file_system.hpp>
#include "utils.hpp"

using namespace ray::renderer::directx11::utilities;

namespace ray::renderer::directx11
{

#define CreateShader(ShaderType, ShaderBuffer, pShader) \
	{ \
		GetDevice()->Create##ShaderType##Shader(ShaderBuffer->GetBufferPointer(), ShaderBuffer->GetBufferSize(), NULL, & pShader); \
	}

	ShaderDX::ShaderDX()
		: m_VertexShader(nullptr)
		, m_PixelShader(nullptr)
		, m_Layout(nullptr)
		, m_MatrixBuffer(nullptr)
	{}

	ShaderDX::~ShaderDX()
	{

	}

	void ShaderDX::Compile()
	{

	}

	void ShaderDX::Create(eShaderTypeDX ShaderType, pcstr Source)
	{
		ID3DBlob* Buffer = nullptr;
		ID3DBlob* Error = nullptr;
		
		switch (ShaderType)
		{
		case ray::renderer::directx11::eShaderTypeDX::Vertex:
			Compile(Source, "VertexMain", "vs_5_0", Buffer, Error);
			CreateShader(Vertex, Buffer, m_VertexShader)
			break;
		case ray::renderer::directx11::eShaderTypeDX::Hull:
			Compile(Source, "HullMain", "hs_5_0", Buffer, Error);
			CreateShader(Hull, Buffer, m_HullShader)
			break;
		case ray::renderer::directx11::eShaderTypeDX::Domain:
			Compile(Source, "DomainMain", "ds_5_0", Buffer, Error);
			CreateShader(Domain, Buffer, m_DomainShader)
			break;
		case ray::renderer::directx11::eShaderTypeDX::Geometry:
			Compile(Source, "GeometryMain", "gs_5_0", Buffer, Error);
			CreateShader(Geometry, Buffer, m_GeometryShader)
			break;
		case ray::renderer::directx11::eShaderTypeDX::Pixel:
			Compile(Source, "PixelMain", "ps_5_0", Buffer, Error);
			CreateShader(Pixel, Buffer, m_PixelShader)
			break;
		default:
			break;
		}

		Buffer->Release();
		Buffer = nullptr;

		Error->Release();
		Error = nullptr;
	}

	HRESULT ShaderDX::Compile(pcstr Source, pcstr Entry, pcstr Model, ID3DBlob* Buffer, ID3DBlob* Error)
	{
		HRESULT hr = D3DCompileFromFile((WCHAR*)Source, NULL, NULL, Entry, Model, D3D10_SHADER_ENABLE_STRICTNESS, 0,
			&Buffer, &Error);
		if (FAILED(hr))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if (Error)
			{
				OutputShaderErrorMessage(Error, Platform::GetHWND(), (WCHAR*)Source);
			}
			// If there was nothing in the error message then it simply could not find the file itself.
			else
			{
				MessageBox(Platform::GetHWND(), (WCHAR*)Source, L"Missing Shader File", MB_OK);
			}
		}

		return hr;
	}

	void ShaderDX::SetSources(std::string& VertexSrc, std::string& PixelSrc)
	{
		SetSources(VertexSrc.c_str(), PixelSrc.c_str());
	}

	void ShaderDX::SetSources(pcstr VertexSrc, pcstr PixelSrc)
	{
	}

	void ShaderDX::UseShader()
	{
	}

	void ShaderDX::Destroy()
	{
	}

	void ShaderDX::SetMatrixes(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX)
	{
	}

	void ShaderDX::OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*)
	{
	}

}


