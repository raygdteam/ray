#include "pch.hpp"
#include "ShaderDX.hpp"
#include <Core/file_system.hpp>

ID3D11Device* ShaderDX::m_sDevice = nullptr;

ShaderDX::ShaderDX()
{

}

ShaderDX::~ShaderDX()
{

}

void ShaderDX::Compile()
{

}

void ShaderDX::SetVertexShader(ray_string& path)
{
	HRESULT hr = S_OK;

	// Компиляция вершинного шейдера
	*m_VertexBlob = NULL;
	hr = _Compile(path.c_str(), "VertexMain", "vs_4_0", m_VertexBlob);
	if (FAILED(hr))
	{
		spdlog::critical("Error compiling Vertex Shader!");
		return;
	}

	// Создание вершинного шейдера
	hr = m_sDevice->CreateVertexShader((*m_VertexBlob)->GetBufferPointer(), (*m_VertexBlob)->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(hr))
	{
		spdlog::critical("Error creating Vertex Shader!");
		FREE_MEM((*m_VertexBlob))
		return;
	}
}

void ShaderDX::SetPixelShader(ray_string& path)
{
	HRESULT hr = S_OK;

	*m_PixelBlob = NULL;
	hr = _Compile(path.c_str(), "PixelMain", "ps_4_0", m_PixelBlob);
	if (FAILED(hr))
	{
		spdlog::critical("Error compiling Pixel Shader!");
		return;
	}

	hr = m_sDevice->CreatePixelShader((*m_PixelBlob)->GetBufferPointer(), (*m_PixelBlob)->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(hr))
	{
		spdlog::critical("Error creating Pixel Shader!");
		FREE_MEM((*m_PixelBlob))
			return;
	}
}

HRESULT ShaderDX::_Compile(const WCHAR* FileName, LPCSTR Entry, LPCSTR ShaderModel, ID3DBlob** Out)
{
	HRESULT hr = S_OK;

	hr = D3DCompileFromFile(FileName, NULL, NULL,
		Entry, ShaderModel, 0,
		0, Out, NULL);

	//auto file = ray::file_system::read_file(FileName);

	return hr;
}
