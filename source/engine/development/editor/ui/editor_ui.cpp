#include "editor_ui.hpp"

#include <editor/windows/debug_window.hpp>
#include <editor/windows/level_outline.hpp>
#include <editor/windows/level_viewport.hpp>
#include <editor/windows/log_window.hpp>

#include "editor/context/context.hpp"

#include <engine/ui2/ext/imgui.h>
#include <engine/ui2/ext/imgui_internal.h>

EditorUi::EditorUi()
{
	
}

void EditorUi::Initialize(IPlatformWindow* window)
{
	check(_rootObject == nullptr);
	
	_rootObject = new UiRootObject();
	_rootObject->Initialize(window);

	_rootObject->Tick();
	ImGui::DockBuilderRemoveNode(1);
	
	gEditorContext.UiContext.PrimaryDockspaceId = 1;
	check(ImGui::DockBuilderGetNode(1) == nullptr);
	{
		ImGui::DockBuilderAddNode(1, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(1, ImVec2(1280, 720));
		
		u32 dockMain = 1;
		u32 dockRight = ImGui::DockBuilderSplitNode(1, ImGuiDir_Right, 0.25f, nullptr, &dockMain);
		u32 dockRightBottom = ImGui::DockBuilderSplitNode(dockRight, ImGuiDir_Down, 0.20f, nullptr, &dockRight);
		u32 dockLeftTop = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Up, 0.70f, nullptr, &dockMain);
		
		ImGui::DockBuilderDockWindow("Level Outline", dockRight);
		ImGui::DockBuilderDockWindow("Log", dockMain);
		ImGui::DockBuilderDockWindow("Level Viewport", dockLeftTop);
		ImGui::DockBuilderDockWindow("EdDebugWindow", dockRightBottom);
		ImGui::DockBuilderFinish(1);
	}

	ImGui::EndFrame();
	
	_rootObject->AddWindow(new EdDebugWindow());
	_rootObject->AddWindow(new EdLevelViewport());
	_rootObject->AddWindow(new EdLevelOutline());
	_rootObject->AddWindow(new EdLogWindow());
}

void EditorUi::Tick()
{
	_rootObject->Tick();
}

void EditorUi::Render(GraphicsContext& ctx)
{
	_rootObject->RenderAll(ctx);
}
