#include "pipeline_state.hpp"
#include <d3d12.h>


namespace ray::renderer::d3d12
{

	D3D12PipelineState::~D3D12PipelineState()
	{
		if (GetInstance())
			static_cast<ID3D12PipelineState*>(GetInstance())->Release();
	}

}