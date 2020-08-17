#include "renderer.hpp"
#include "command_context.hpp"
#include "command_queue.hpp"

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>

#include <engine/state/state.hpp>

namespace ray::renderer_core_api
{
	namespace globals
	{
		CommandListManager gCommandListManager;
		ContextManager gContextManager;
		ID3D12Device* gDevice;
		DescriptorAllocator gDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES] =
		{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			D3D12_DESCRIPTOR_HEAP_TYPE_DSV
		};
	}


	bool IRenderer::_sbReady = false;

	void IRenderer::Initialize(ray::core::IPlatformWindow* window)
	{
		
	}

	void IRenderer::BeginScene()
	{
		
	} 

	void IRenderer::EndScene()
	{
		
	}

	void IRenderer::Execute()
	{
		
	}


	void IRenderer::WaitForPreviousFrame()
	{
		
	}

	void IRenderer::Shutdown()
	{
		
	}
}
