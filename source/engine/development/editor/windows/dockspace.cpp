#include "dockspace.hpp"



#include "about_window.hpp"
#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"

void EdDockspace::MenuBar()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Close Level", nullptr))
		{
			EditorCommand* cmd = new EditorCommand();
			cmd->Type = eCloseLevel;
			gEditorEngine->RunCommand(cmd);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("About", nullptr))
		{
			_parent->AddWindow(new EdAboutWindow());
		}
		ImGui::EndMenu();
	}
}

void EdDockspace::Tick()
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

	if (ImGui::BeginMenuBar())
	{
		MenuBar();
		ImGui::EndMenuBar();
	}
	
	ImGui::DockSpace(1, ImVec2(0, 0));
	ImGui::End();
}

