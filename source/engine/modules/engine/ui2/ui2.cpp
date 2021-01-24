#include "ui2.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/ui_renderer.hpp>
#include "ext/imgui.h"
#include "ext/imgui_impl_win32.h"
#include "renderer_core/resources/buffer_manager.hpp"


UiObject::UiObject()
{ }

void UiRootObject::Initialize(IPlatformWindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window->GetWindowHandleRaw());

	unsigned char* pixels = nullptr;
	int width, height;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	UiRenderer::Initialize(u32(width), u32(height), pixels);
}

void UiRootObject::Tick()
{
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (UiWindow* window : _windows)
	{
		for (UiObject* object : window->_objects)
		{
			object->Tick();
		}
	}
	
	ImGui::EndFrame();
}

void UiRootObject::RenderAll()
{
	/*GraphicsContext& ctx = GraphicsContext::Begin();
	UiRenderer::Begin();
	UiRenderer::End(ctx);
	ctx.Finish(true);*/
}
