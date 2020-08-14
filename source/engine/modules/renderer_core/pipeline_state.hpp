#pragma once

namespace ray::renderer_core_api
{
	class PipelineState
	{
	public:
		ID3D12PipelineState* GetPSO() const noexcept { return _pipelineState; }

	protected:
		ID3D12PipelineState* _pipelineState;
	};

	class GraphicsPipeline : public PipelineState
	{

	};

	class ComputePipeline : public PipelineState
	{

	};
}