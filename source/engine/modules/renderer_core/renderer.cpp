#include "renderer.hpp"
#include "renderer_globals.hpp"

//ñäåëàéòå óæå èíòåðôåéñ äëÿ ðàáîòû ñ ìîäóëÿìè
#include <Windows.h>

#include <engine/state/state.hpp>

bool IRenderer::_sbReady = false;

namespace ray::renderer_core_api
{

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
