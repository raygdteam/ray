#include "../renderer_core/renderer.hpp"

class RendererD3D11 : public ray::IRenderer
{
	void Initialize(void*) override;
	void Draw() override;
};

extern "C" __declspec(dllexport) ray::IRenderer * GetRendererApi()
{
	return new RendererD3D11;
}

void RendererD3D11::Initialize(void* window)
{

}

void RendererD3D11::Draw()
{

}