#include "ui2.hpp"
#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/ui_renderer.hpp>
#include "ext/imgui.h"
#include "ext/imgui_impl_win32.h"
#include "ext/imgui_internal.h"
#include "renderer_core/resources/buffer_manager.hpp"


UiObject::UiObject(UiWindow* window)
{
	_parent = window;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void ImguiCallback(void* hWnd, u32 msg, u64 wParam, s64 lParam)
{
	ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
}

void UiWindow::TickBegin()
{
	u32 pushCount = 0x0;
	
	if (Size.x != 0.0f && Size.y != 0.0f)
	{
		ImGui::SetNextWindowSize({ Size.x, Size.y }, ImGuiCond_FirstUseEver);
	}
	
	if (Padding.x != -1.0f && Padding.y != -1.0f)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Padding.x, Padding.y));
		pushCount++;
	}

	ImGui::Begin(Title.AsRawStr());
	{
		Size.x = ImGui::GetWindowWidth();
		Size.y = ImGui::GetWindowHeight();
		
		Tick();

		for (UiObject* object : _objects)
		{
			object->Tick();
		}

		LateTick();
	}
	
	ImGui::End();
	ImGui::PopStyleVar(pushCount);
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

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = nullptr;

	//ImGui::GetCurrentContext()->SettingsLoaded = true;
}

void UiRootObject::Tick()
{
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	//ImGui::ShowDemoWindow();

	RunDockspace();
	
	for (UiWindow* window : _windows)
	{
		window->TickBegin();
	}
}

void UiRootObject::RunDockspace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DOCKSPACE", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGui::DockSpace(1, ImVec2(0, 0));
	ImGui::End();
}

void UiRootObject::RenderAll(GraphicsContext& gfxContext)
{
	static_assert(sizeof(ImDrawVert) == sizeof(UiVertex), "size mismatch");
	static_assert(sizeof(ImDrawIdx) == sizeof(u32), "size mismatch");

	ImGui::Render();
	ImDrawData* data = ImGui::GetDrawData();
	if (data->TotalVtxCount <= 0) return;
	
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
			
			D3D12_RECT r = {
				.left = (LONG)(draw->ClipRect.x - clipOff.x),
				.top = (LONG)(draw->ClipRect.y - clipOff.y),
				.right = (LONG)(draw->ClipRect.z - clipOff.x),
				.bottom = (LONG)(draw->ClipRect.w - clipOff.y)
			};
			
			if (!(r.right > r.left && r.bottom > r.top))
				continue;
			
			gfxContext.SetScissor(r);
			UiRenderer::Draw(draw->ElemCount, draw->VtxOffset + vertexOffset, draw->IdxOffset + indexOffset, (u32)draw->TextureId, gfxContext);
		}

		vertexOffset += cmd->VtxBuffer.Size;
		indexOffset += cmd->IdxBuffer.Size;
	}

	//FVector3 pos = { 500.f, 100.f, 0.f };
	//FVector2 size = { 4.f, 3.f };
	//UiRenderer::DrawSceneRenderTarget(pos, size, gfxContext);

	UiRenderer::End(gfxContext);
}

void UiRootObject::AddWindow(UiWindow* window)
{
	_windows.PushBack(window);
	if (window->IsDockingEnabled())
		window->DockingOneTimeSetup();
}

void UiRootObject::SetDockspaceEnabled(bool state)
{
	_dockspaceEnabled = state;
}
