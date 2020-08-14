#include "renderer_2d.hpp"
#include "ray_renderer_core_class_helper.hpp"
#include <engine/state/state.hpp>

namespace ray::renderer_core_api
{
	IRRCClassHelper* gClassHelper = nullptr;

	struct QuadVertex
	{
		FVector<3> Position;
		FVector<4> Color;
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

	void Renderer2D::DrawQuad(FVector<3> pos, FVector<4> color)
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