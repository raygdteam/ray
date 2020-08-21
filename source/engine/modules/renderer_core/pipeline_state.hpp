#pragma once
#include <core/debug/assert.hpp>
#include <d3d12.h>
#include "d3dx12.h"
#include <memory>

namespace ray::renderer_core_api
{
	class RootSignature;
	class CommandContext;

	class PipelineState
	{
	public:
		PipelineState() 
			: _rootSignature(nullptr)
		{}

		static void DestroyAll() {}

		void SetRootSignature(const RootSignature& rootSignature) noexcept
		{
			_rootSignature = &rootSignature;
		}

		const RootSignature& GetRootSignature() const noexcept
		{
			check(_rootSignature != nullptr)
			return *_rootSignature;
		}

		ID3D12PipelineState* GetPSO() const noexcept { return _pipelineState; }

	protected:
		ID3D12PipelineState* _pipelineState;
		const RootSignature* _rootSignature;
	};

	class GraphicsPipeline : public PipelineState
	{
		friend class CommandContext;

	public:
		GraphicsPipeline();

		void SetBlendState(const D3D12_BLEND_DESC& blendDesc);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc);
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
		void SetSampleMask(u32 sampleMask);
		void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType);
		void SetRenderTargetFormat(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, u32 msaaCount = 1, u32 msaaQuality = 0) { SetRenderTargetFormats(1, &rtvFormat, dsvFormat, msaaCount, msaaQuality); }
		void SetRenderTargetFormats(u32 numRTVs, const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, u32 msaaCount = 1, u32 msaaQuality = 0);
		void SetInputLayout(u32 numElements, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs);
		void SetPrimitiveRestart(D3D12_INDEX_BUFFER_STRIP_CUT_VALUE ibProps);

		void SetVertexShader(const void* binary, size_t size) { _pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary), size); }
		void SetPixelShader(const void* binary, size_t size) { _pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary), size); }
		void SetGeometryShader(const void* binary, size_t size) { _pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary), size); }
		void SetHullShader(const void* binary, size_t size) { _pipelineDesc.HS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary), size); }
		void SetDomainShader(const void* binary, size_t size) { _pipelineDesc.DS = CD3DX12_SHADER_BYTECODE(const_cast<void*>(binary), size); }

		void SetVertexShader(const D3D12_SHADER_BYTECODE& binary) { _pipelineDesc.VS = binary; }
		void SetPixelShader(const D3D12_SHADER_BYTECODE& binary) { _pipelineDesc.PS = binary; }
		void SetGeometryShader(const D3D12_SHADER_BYTECODE& binary) { _pipelineDesc.GS = binary; }
		void SetHullShader(const D3D12_SHADER_BYTECODE& binary) { _pipelineDesc.HS = binary; }
		void SetDomainShader(const D3D12_SHADER_BYTECODE& binary) { _pipelineDesc.DS = binary; }

		void Finalize();

	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC _pipelineDesc;
		std::shared_ptr<const D3D12_INPUT_ELEMENT_DESC> _inputLayouts;
	};

	class ComputePipeline : public PipelineState
	{

	};
}