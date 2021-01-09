#include "ui2.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>
#include "ext/imgui.h"
#include "ext/imgui_impl_win32.h"

using namespace ray::renderer_core_api;

void UiButton::Tick()
{
	ImGui::Button({});
}

void UiRootObject::Initialize(IPlatformWindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window->GetWindowHandleRaw());
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
	// GraphicsContext& ctx = GraphicsContext::Begin();
	// ctx.Finish(true);
}
