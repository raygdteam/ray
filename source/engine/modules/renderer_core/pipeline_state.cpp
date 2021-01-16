#include "pipeline_state.hpp"
#include "root_signature.hpp"
#include "renderer.hpp"
#include <cstring>

GraphicsPipeline::GraphicsPipeline()
{
	memset(&_pipelineDesc, 0, sizeof(_pipelineDesc));
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.NodeMask = 1;
	_pipelineDesc.SampleMask = 0xFFFFFFFFu;
	_pipelineDesc.InputLayout.NumElements = 0;
}

void GraphicsPipeline::SetBlendState(const D3D12_BLEND_DESC& blendDesc)
{
	_pipelineDesc.BlendState = blendDesc;
}

void GraphicsPipeline::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc)
{
	_pipelineDesc.RasterizerState = rasterizerDesc;
}

void GraphicsPipeline::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	_pipelineDesc.DepthStencilState = depthStencilDesc;
}

void GraphicsPipeline::SetDSVFormat(DXGI_FORMAT format)
{
	_pipelineDesc.DSVFormat = format;
}

void GraphicsPipeline::SetSampleMask(u32 sampleMask)
{
	_pipelineDesc.SampleMask = sampleMask;
}

void GraphicsPipeline::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
{
	ray_assert(topologyType != D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED, "Can't draw with undefined topology")
		_pipelineDesc.PrimitiveTopologyType = topologyType;
}

void GraphicsPipeline::SetRenderTargetFormats(u32 numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, u32 msaaCount, u32 msaaQuality)
{
	ray_assert(numRTVs == 0 || rtvFormats != nullptr, "null format array conflicts with non-zero length")
		for (u32 i = 0; i < numRTVs; ++i)
			_pipelineDesc.RTVFormats[i] = rtvFormats[i];
	for (u32 i = numRTVs; i < _pipelineDesc.NumRenderTargets; ++i)
		_pipelineDesc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
	_pipelineDesc.NumRenderTargets = numRTVs;
	_pipelineDesc.DSVFormat = dsvFormat;
	_pipelineDesc.SampleDesc.Count = msaaCount;
	_pipelineDesc.SampleDesc.Quality = msaaQuality;
}

void GraphicsPipeline::SetInputLayout(u32 numElements, const D3D12_INPUT_ELEMENT_DESC* inputLayout)
{
	_pipelineDesc.InputLayout.NumElements = numElements;
	if (numElements > 0)
	{
		D3D12_INPUT_ELEMENT_DESC* newElements = new D3D12_INPUT_ELEMENT_DESC[numElements]; //static_cast<D3D12_INPUT_ELEMENT_DESC*>(malloc(sizeof(D3D12_INPUT_ELEMENT_DESC) * numElements));
		memcpy(newElements, inputLayout, sizeof(D3D12_INPUT_ELEMENT_DESC) * numElements);
		_inputLayouts.reset(static_cast<const D3D12_INPUT_ELEMENT_DESC*>(newElements));
	}
	else
		_inputLayouts = nullptr;
}

void GraphicsPipeline::SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ibProps)
{
}

void GraphicsPipeline::Finalize()
{
	_pipelineDesc.pRootSignature = _rootSignature->GetRootSignature();
	check(_pipelineDesc.pRootSignature != nullptr)
		_pipelineDesc.InputLayout.pInputElementDescs = _inputLayouts.get();
	auto hr = gDevice->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
	check(hr == S_OK)
}
