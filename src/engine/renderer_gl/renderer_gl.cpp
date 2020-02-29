#include "../renderer_core/renderer.hpp"

class RendererGL : public ray::IRenderer
{
public:
	void Initialize(void* window) override {}
	void Draw() override {}
};


extern "C" __declspec(dllexport) ray::IRenderer*  GetRendererApi()
{
	return new RendererGL();
}