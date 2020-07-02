#include "renderer_commands.hpp"


namespace ray::renderer_core_api
{
	bool RendererCommands::Initialize(IRenderer* renderer, PrimitiveTopology topology)
	{
		_device = renderer->_device;


		return true;
	}

	void RendererCommands::Draw(Vertex* vertices)
	{

	}

	void RendererCommands::DrawIndexed(Vertex* vertices, u32* indices)
	{

	}
}