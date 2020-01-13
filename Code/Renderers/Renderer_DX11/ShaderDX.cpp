#include "pch.hpp"
#include "ShaderDX.hpp"
#include <Core/file_system.hpp>
#include "utils.hpp"

using namespace ray::renderer::directx11::utilities;

namespace ray::renderer::directx11
{

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

	void ShaderDX::SetSources(std::string& VertexSrc, std::string& PixelSrc)
	{
		SetSources(VertexSrc.c_str(), PixelSrc.c_str());
	}

	void ShaderDX::SetSources(pcstr VertexSrc, pcstr PixelSrc)
	{
		HRESULT result;
		ID3D10Blob* errorMessage;
		ID3D10Blob* vertexShaderBuffer;
		ID3D10Blob* pixelShaderBuffer;
		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		u32 numElements;
		D3D11_BUFFER_DESC matrixBufferDesc;


		// Initialize the pointers this function will use to null.
		errorMessage = 0;
		vertexShaderBuffer = 0;
		pixelShaderBuffer = 0;

		// Compile the vertex shader code.
		result = D3DCompileFromFile((WCHAR*)VertexSrc, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
			&vertexShaderBuffer, &errorMessage);
		if (FAILED(result))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if (errorMessage)
			{
				OutputShaderErrorMessage(errorMessage, Platform::GetHWND(), (WCHAR*)VertexSrc);
			}
			// If there was  nothing in the error message then it simply could not find the shader file itself.
			else
			{
				MessageBox(Platform::GetHWND(), (WCHAR*)VertexSrc, L"Missing Shader File", MB_OK);
			}

			return;
		}

		// Compile the pixel shader code.
		result = D3DCompileFromFile((WCHAR*)PixelSrc, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
			&pixelShaderBuffer, &errorMessage);
		if (FAILED(result))
		{
			// If the shader failed to compile it should have writen something to the error message.
			if (errorMessage)
			{
				OutputShaderErrorMessage(errorMessage, Platform::GetHWND(), (WCHAR*)PixelSrc);
			}
			// If there was nothing in the error message then it simply could not find the file itself.
			else
			{
				MessageBox(Platform::GetHWND(), (WCHAR*)PixelSrc, L"Missing Shader File", MB_OK);
			}
			return;
		}

		// Create the vertex shader from the buffer.
		result = GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
		if (FAILED(result))
		{
			return;
		}

		// Create the pixel shader from the buffer.
		result = GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
		if (FAILED(result))
		{
			return;
		}
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


