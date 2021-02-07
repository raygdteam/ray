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
	(void)io.DisplaySize;
}

void UiRootObject::Tick()
{
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool data = {};

	ImGui::ShowDemoWindow();
	
	ImGui::Begin("aaa");
	ImGui::Text("text");
	ImGui::Button("button");
	ImGui::Checkbox("checkbox", &data);
	ImGui::Separator();
	ImGui::RadioButton("RadioButton", true);
	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetOverlayDrawList()->AddCircleFilled(io.MousePos, 10.f, IM_COL32_WHITE);
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
	static_assert(sizeof(ImDrawVert) == sizeof(UiVertex), "size mismatch");
	static_assert(sizeof(ImDrawIdx) == sizeof(u32), "size mismatch");

	ImGui::Render();
	ImDrawData* data = ImGui::GetDrawData();
	if (data->TotalVtxCount < 0) return;
	
	//u32 framebufferWidth = (u32)(data->DisplaySize.x * data->FramebufferScale.x);
	//u32 framebufferHeight = (u32)(data->DisplaySize.y * data->FramebufferScale.y);

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
	
	ImDrawList* cmd = nullptr;
	for (int i = 0; i < data->CmdListsCount; ++i)
	{
		cmd = data->CmdLists[i];
		UiRenderer::SetVertices(cmd->VtxBuffer.Data, cmd->VtxBuffer.Size);
		UiRenderer::SetIndices(cmd->IdxBuffer.Data, cmd->IdxBuffer.Size);
	}

	UiRenderer::Begin(vp, gfxContext);

	ImVec2 clipOff = data->DisplayPos;
	
	size_t vertexOffset = 0;
	size_t indexOffset = 0;
	for (int i = 0; i < data->CmdListsCount; ++i)
	{
		cmd = data->CmdLists[i];

		for (int n = 0; n < cmd->CmdBuffer.Size; n++)
		{
			ImDrawCmd* draw = &cmd->CmdBuffer[n];
			
			ImVec4 clipRect;
			clipRect.x = (draw->ClipRect.x - clipOff.x);
			clipRect.y = (draw->ClipRect.y - clipOff.y);
			clipRect.z = (draw->ClipRect.z - clipOff.x);
			clipRect.w = (draw->ClipRect.w - clipOff.y);

			D3D12_RECT r = { (LONG)(draw->ClipRect.x - clipOff.x), (LONG)(draw->ClipRect.y - clipOff.y), (LONG)(draw->ClipRect.z - clipOff.x), (LONG)(draw->ClipRect.w - clipOff.y) };
			if (!(r.right > r.left && r.bottom > r.top))
				continue;
			
			gfxContext.SetScissor(r);
			UiRenderer::Draw(draw->ElemCount, draw->VtxOffset + vertexOffset, draw->IdxOffset + indexOffset, gfxContext);
		}

		vertexOffset += cmd->VtxBuffer.Size;
		indexOffset += cmd->IdxBuffer.Size;
	}	
	UiRenderer::End(gfxContext);
}
