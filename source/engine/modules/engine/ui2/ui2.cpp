#include "ui2.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/ui_renderer.hpp>
#include "ext/imgui.h"
#include "ext/imgui_impl_win32.h"
#include "renderer_core/resources/buffer_manager.hpp"


UiObject::UiObject()
{ }

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void ImguiCallback(void* hWnd, u32 msg, u64 wParam, s64 lParam)
{
	ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
}
void UiRootObject::Initialize(IPlatformWindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window->GetWindowHandleRaw());

	window->RegisterEventCallback(ImguiCallback);

	unsigned char* pixels = nullptr;
	int width, height;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	UiRenderer::Initialize(u32(width), u32(height), pixels);

	io.BackendRendererName = "imgui_impl_vulkan";
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.

}

void UiRootObject::Tick()
{
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("aaa");
	ImGui::Text("aaaaa");
	ImGui::End();
	
	/*for (UiWindow* window : _windows)
	{
		for (UiObject* object : window->_objects)
		{
			object->Tick();
		}
	}*/
	
	ImGui::EndFrame();
}

void UiRootObject::RenderAll(GraphicsContext& gfxContext)
{
	ImGui::Render();
	ImDrawData* data = ImGui::GetDrawData();
	if (data->TotalVtxCount < 0) return;
	
	float L = data->DisplayPos.x;
	float R = data->DisplayPos.x + data->DisplaySize.x;
	float T = data->DisplayPos.y;
	float B = data->DisplayPos.y + data->DisplaySize.y;
	float mvp[4][4] =
	{
		{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
		{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
		{ 0.0f,         0.0f,           0.5f,       0.0f },
		{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
	};
	FMatrix4x4 vp = *(FMatrix4x4*)&mvp;
	
	UiRenderer::Begin();
	
	ImDrawList* cmd = nullptr;

	u32 vtxOffset = 0;
	u32 idxOffset = 0;
	
	for (int i = 0; i < data->CmdListsCount; ++i)
	{
		cmd = data->CmdLists[i];

		for (int n = 0; n < cmd->CmdBuffer.Size; n++)
		{
			ImDrawCmd* draw = &cmd->CmdBuffer[n];
			
			static_assert(sizeof(ImDrawVert) == sizeof(UiVertex), "size mismatch");

			UiRenderer::Draw((UiVertex*)cmd->VtxBuffer.Data + vtxOffset, draw->ElemCount / 3, cmd->IdxBuffer.Data + idxOffset, draw->ElemCount, gfxContext);
		}
		vtxOffset += cmd->VtxBuffer.Size;
		idxOffset += cmd->IdxBuffer.Size;
	}
	
	UiRenderer::End(gfxContext);
}
