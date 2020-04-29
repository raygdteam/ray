#include "../renderer_core/renderer.hpp"

class RendererGL : public ray::IRenderer
{
public:
	void Initialize(ray::core::IPlatformWindow* window) override {}
	void Draw() override {}
	void Shutdown() override {}
};


extern "C" __declspec(dllexport) ray::IRenderer*  GetRendererApi()
{
	return new RendererGL();
}