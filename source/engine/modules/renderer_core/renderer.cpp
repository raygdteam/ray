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
	}


	bool IRenderer::_sbReady = false;

	void IRenderer::Initialize(ray::core::IPlatformWindow* window, IModule* rendererModule)
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
