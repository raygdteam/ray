#include "editor_ui.hpp"



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
	
	_levelChooser = new LevelChooser();
	_rootObject->AddWindow(_levelChooser);

	_debugWindow = new EdDebugWindow();
	_levelViewport = new EdLevelViewport();
	_levelOutline = new EdLevelOutline();
	_logWindow = new EdLogWindow();
}

void EditorUi::Tick()
{
	_rootObject->Tick();
	
	if (gShowDemoWindow)
		ImGui::ShowDemoWindow(&gShowDemoWindow);
}

void EditorUi::Render(GraphicsContext& ctx)
{
	_rootObject->RenderAll(ctx);
}

void EditorUi::CmdLevelLoaded()
{
	if (_levelChooser == nullptr) return;
	
	_rootObject->AddWindow(_debugWindow);
	_rootObject->AddWindow(_levelViewport);
	_rootObject->AddWindow(_levelOutline);
	_rootObject->AddWindow(_logWindow);

	_rootObject->RemoveWindow(_levelChooser);
	delete _levelChooser;
	_levelChooser = nullptr;
}

void EditorUi::CmdCloseLevel()
{
	_rootObject->RemoveWindow(_debugWindow);
	_rootObject->RemoveWindow(_levelViewport);
	_rootObject->RemoveWindow(_levelOutline);
	_rootObject->RemoveWindow(_logWindow);

	_levelChooser = new LevelChooser();
	_rootObject->AddWindow(_levelChooser);
}
