#include "pch.hpp"
#include "RendererDX.hpp"

struct SimpleVertex
{
	DirectX::XMVECTORI32 Pos;
};

RendererDX::RendererDX()
	: m_DriverType(D3D_DRIVER_TYPE_NULL)
	, m_FeatureLevel(D3D_FEATURE_LEVEL_11_0)
	, m_Device(nullptr)
	, m_Context(nullptr)
	, m_SwapChain(nullptr)
	, m_RenderTargetView(nullptr)
{
}

bool RendererDX::Init()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(Platform::GetHWND(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = Platform::GetHWND();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, &m_FeatureLevel, &m_Context);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	
	FREE_MEM(pBackBuffer)

	if (FAILED(hr))
		return false;

	m_Context->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_Context->RSSetViewports(1, &vp);

	ShaderDX::SetDevice(m_Device);
	ID3DBlob* p;
	ID3DBlob* v;
	shader.SetVertexBlob(&v);
	shader.SetPixelBlob(&p);
	ray_string src;
	src = TEXT("shader.hlsl");
	shader.SetVertexShader(src);
	shader.SetPixelShader(src);

	m_Context->VSSetShader(shader.GetVertexShader(), 0, 0);
	m_Context->PSSetShader(shader.GetPixelShader(), 0, 0);

	// определение входного формата
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// создание входного формата
	hr = m_Device->CreateInputLayout(layout, numElements, v->GetBufferPointer(), v->GetBufferSize(), &m_VertexLayout);

	FREE_MEM(v)
	if (FAILED(hr))
		return false;

	// установка входного формата
	m_Context->IASetInputLayout(m_VertexLayout);

	// Создание буфера вершин
	float vertices[] =
	{
		0.0f, 0.5f, 0.5f, 1.f,
		0.5f, -0.5f, 0.5f, 1.f,
		-0.5f, -0.5f, 0.5f, 1.f
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = vertices;

	hr = m_Device->CreateBuffer(&bd, &Data, &m_VertexBuffer);
	if (FAILED(hr))
		return false;

	// Установка вершинного буфера
	UINT stride = 12;//sizeof(float);
	UINT offset = 0;
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// установка топологии примитива
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void RendererDX::Draw()
{
	// Установка вершинного буфера
	UINT stride = 12;//sizeof(float);
	UINT offset = 0;
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// установка топологии примитива
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Context->VSSetShader(shader.GetVertexShader(), NULL, 0);
	m_Context->PSSetShader(shader.GetPixelShader(), NULL, 0);
	
	//m_Context->Draw(3, 0);
	m_Context->DrawIndexed(3, 0, 0);
}

void RendererDX::Destroy()
{
	if (m_Context)
		m_Context->ClearState();

	FREE_MEM(m_RenderTargetView)
	FREE_MEM(m_SwapChain)
	FREE_MEM(m_Context)
	FREE_MEM(m_Device)
}

void RendererDX::BeginFrame()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_Context->ClearRenderTargetView(m_RenderTargetView, ClearColor);
}

void RendererDX::EndFrame()
{
	m_SwapChain->Present(0, 0);
}
