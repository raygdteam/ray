#pragma once
#include <renderer_core/pipeline_state.hpp>

using namespace ray::renderer_core_api;

namespace ray::renderer::d3d12
{

    class D3D12PipelineState :
        public IPipelineState
    {
    public:
        D3D12PipelineState() {}
        ~D3D12PipelineState()

    };

}

