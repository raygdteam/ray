#include "renderer_2d.hpp"
#include "ray_renderer_core_class_helper.hpp"
#include <engine/state/state.hpp>

namespace ray::renderer_core_api
{
	IRRCClassHelper* gClassHelper = nullptr;

	struct QuadVertex
	{
		math::Vector<3, float> Position;
		math::Vector<4, float> Color;
	};


	void Renderer2D::Initialize()
	{
		
	}

	void Renderer2D::Begin()
	{
		
	}

	void Renderer2D::End()
	{
		Flush();
	}

	void Renderer2D::DrawQuad(math::Vector<3, float> pos, math::Vector<4, float> color)
	{
		

	}

	void Renderer2D::Flush()
	{

	}

	void Renderer2D::FlushAndReset()
	{
		Flush();
		Begin();
	}

	Renderer2D::~Renderer2D()
	{

	}
}