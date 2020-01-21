#include "pch.hpp"
#include "ShaderDX.hpp"
#include <core/file_system.hpp>
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
		, m_VertexShaderBuffer(nullptr)
	{}

	ShaderDX::~ShaderDX() {}

	void ShaderDX::Create(eShaderTypeDX ShaderType, const WCHAR* Source)
	{
		ID3DBlob* Buffer = nullptr;
		ID3DBlob* Error = nullptr;

		switch (ShaderType)
		{
		case eShaderTypeDX::Vertex:
			Compile(Source, "VertexMain", "vs_5_0", &Buffer, Error);
			CreateShader(Vertex, Buffer, m_VertexShader)
			m_VertexShaderBuffer = Buffer;
			break;
		case eShaderTypeDX::Hull:
			Compile(Source, "HullMain", "hs_5_0", &Buffer, Error);
			CreateShader(Hull, Buffer, m_HullShader)
			break;
		case eShaderTypeDX::Domain:
			Compile(Source, "DomainMain", "ds_5_0", &Buffer, Error);
			CreateShader(Domain, Buffer, m_DomainShader)
			break;
		case eShaderTypeDX::Geometry:
			Compile(Source, "GeometryMain", "gs_5_0", &Buffer, Error);
			CreateShader(Geometry, Buffer, m_GeometryShader)
			break;
		case eShaderTypeDX::Pixel:
			Compile(Source, "PixelMain", "ps_5_0", &Buffer, Error);
			CreateShader(Pixel, Buffer, m_PixelShader)
			break;
		default:
			break;

			Buffer->Release();
			Buffer = nullptr;

			Error->Release();
			Error = nullptr;

		}
	}
	
	HRESULT ShaderDX::Compile(const WCHAR* Source, pcstr Entry, pcstr Model, ID3DBlob** Buffer, ID3DBlob* Error)
	{
		HRESULT hr = D3DCompileFromFile(Source, NULL, NULL, Entry, Model, D3D10_SHADER_ENABLE_STRICTNESS, 0, Buffer, &Error);
		if (FAILED(hr))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if (Error)
			{
				spdlog::critical("Error compiling shader\n {}", (pcstr)Error->GetBufferPointer());
			}
			// If there was nothing in the error message then it simply could not find the file itself.
			else
			{
				MessageBox(Platform::GetHWND(), (WCHAR*)Source, L"Missing Shader File", MB_OK);
			}
		}

		return hr;
	}

	void ShaderDX::UseShader()
	{
		// Set the vertex input layout.
		GetDeviceContext()->IASetInputLayout(m_Layout);

		// Set the vertex and pixel shaders that will be used to render this triangle.
		GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		// Render the triangle.
		GetDeviceContext()->DrawIndexed(m_IndexCount, 0, 0);

	}

	void ShaderDX::Destroy()
	{
		// Release the matrix constant buffer.
		if (m_MatrixBuffer)
		{
			m_MatrixBuffer->Release();
			m_MatrixBuffer = 0;
		}

		// Release the layout.
		if (m_Layout)
		{
			m_Layout->Release();
			m_Layout = 0;
		}

		// Release the pixel shader.
		if (m_PixelShader)
		{
			m_PixelShader->Release();
			m_PixelShader = 0;
		}

		// Release the geometry shader.
		if (m_GeometryShader)
		{
			m_GeometryShader->Release();
			m_GeometryShader = 0;
		}

		// Release the domain shader.
		if (m_DomainShader)
		{
			m_DomainShader->Release();
			m_DomainShader = 0;
		}

		// Release the hull shader.
		if (m_HullShader)
		{
			m_HullShader->Release();
			m_HullShader = 0;
		}

		// Release the vertex shader.
		if (m_VertexShader)
		{
			m_VertexShader->Release();
			m_VertexShader = 0;
		}

	}

	void ShaderDX::SetParams(size_t IndexCount, DirectX::XMMATRIX Model, DirectX::XMMATRIX World, DirectX::XMMATRIX Projection)
	{
		m_IndexCount = IndexCount;
	}

	void ShaderDX::SetInputElement(D3D11_INPUT_ELEMENT_DESC* InputElementDesc, size_t NumElements)
	{
		// Create the vertex input layout.
		HRESULT hr;

		hr = GetDevice()->CreateInputLayout(InputElementDesc, NumElements, m_VertexShaderBuffer->GetBufferPointer(),
			m_VertexShaderBuffer->GetBufferSize(), &m_Layout);
		if (FAILED(hr))
		{
			spdlog::critical("renderer_dx: Error creating input layout");
			return;
		}
		
		// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
		m_VertexShaderBuffer->Release();
		m_VertexShaderBuffer = 0;
	}

}


