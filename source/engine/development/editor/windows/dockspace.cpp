#include "dockspace.hpp"

#include <core/core.hpp>
#include <windows.h>
#include <commdlg.h>

#include "about_window.hpp"
#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"

void EdDockspace::MenuBar()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Open Level...", nullptr))
		{
			OPENFILENAMEA openFileName = {};

			char fileName[MAX_PATH] = {};

			openFileName.lStructSize = sizeof(openFileName);
			openFileName.hwndOwner = NULL;
			openFileName.lpstrFilter = "Json Files (*.json)\0*.json\0";
			openFileName.lpstrFile = fileName;
			openFileName.nMaxFile = MAX_PATH;
			openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			openFileName.lpstrDefExt = "";

			if (GetOpenFileNameA(&openFileName))
			{
				EditorCommand* cmd1 = new EditorCommand();
				cmd1->Type = eCloseLevel;
				gEditorEngine->RunCommand(cmd1);

				EditorCommand_LoadLevel* cmd = new EditorCommand_LoadLevel;
				cmd->Path = String(fileName);
				gEditorEngine->RunCommand(cmd);
			}
		}

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

