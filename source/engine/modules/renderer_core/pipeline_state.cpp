#include "pipeline_state.hpp"
#include "root_signature.hpp"
#include "renderer.hpp"
#include <cstring>

GraphicsPipeline::GraphicsPipeline() noexcept
{
	memset(&_pipelineDesc, 0, sizeof(_pipelineDesc));
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.NodeMask = 1;
	_pipelineDesc.SampleMask = 0xFFFFFFFFu;
	_pipelineDesc.InputLayout.NumElements = 0;
}

GraphicsPipeline::~GraphicsPipeline()
{
	delete[] _inputLayouts;
}

void GraphicsPipeline::SetBlendState(const D3D12_BLEND_DESC& blendDesc) noexcept
{
	_pipelineDesc.BlendState = blendDesc;
}

void GraphicsPipeline::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc) noexcept
{
	_pipelineDesc.RasterizerState = rasterizerDesc;
}

void GraphicsPipeline::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc) noexcept
{
	_pipelineDesc.DepthStencilState = depthStencilDesc;
}

void GraphicsPipeline::SetDSVFormat(DXGI_FORMAT format) noexcept
{
	_pipelineDesc.DSVFormat = format;
}

void GraphicsPipeline::SetSampleMask(u32 sampleMask) noexcept
{
	_pipelineDesc.SampleMask = sampleMask;
}

void GraphicsPipeline::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType) noexcept
{
	ray_assert(topologyType != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED, "Can't draw with undefined topology")
		_pipelineDesc.PrimitiveTopologyType = topologyType;
}

void GraphicsPipeline::SetRenderTargetFormats(u32 numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, u32 msaaCount, u32 msaaQuality) noexcept
{
	ray_assert(numRTVs == 0 || rtvFormats != nullptr, "null format array conflicts with non-zero length")
	for (u32 i = 0; i < numRTVs; ++i)
		_pipelineDesc.RTVFormats[i] = rtvFormats[i];
	_pipelineDesc.NumRenderTargets = numRTVs;
	_pipelineDesc.DSVFormat = dsvFormat;
	_pipelineDesc.SampleDesc.Count = msaaCount;
	_pipelineDesc.SampleDesc.Quality = msaaQuality;
}

void GraphicsPipeline::SetInputLayout(u32 numElements, const D3D12_INPUT_ELEMENT_DESC* inputLayout) noexcept
{
	_pipelineDesc.InputLayout.NumElements = numElements;
	if (numElements > 0)
	{
		_inputLayouts = new D3D12_INPUT_ELEMENT_DESC[numElements]; 
		memcpy(_inputLayouts, inputLayout, sizeof(D3D12_INPUT_ELEMENT_DESC) * numElements);
	}
	else
		_inputLayouts = nullptr;
}

void GraphicsPipeline::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ibProps) noexcept
{
}

void GraphicsPipeline::Finalize() noexcept
{
	_pipelineDesc.pRootSignature = _rootSignature->GetRootSignature();
	check(_pipelineDesc.pRootSignature != nullptr)
		_pipelineDesc.InputLayout.pInputElementDescs = _inputLayouts;
	auto hr = gDevice->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
	check(hr == S_OK)
}
