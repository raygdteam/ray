#include "dockspace.hpp"

#ifdef RAY_PLATFORM_WIN
	#include <windows.h>
	#include <commdlg.h>
#endif

#include "about_window.hpp"
#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"

void EdDockspace::MenuBar()
{
#ifdef RAY_PLATFORM_WIN
	if (ImGui::BeginMenu("Level"))
	{
		if (ImGui::MenuItem("Open", nullptr))
		{
			OPENFILENAME open_file_name = {};

			char file_name[MAX_PATH] = {};

			open_file_name.lStructSize = sizeof(open_file_name);
			open_file_name.hwndOwner = NULL;
			open_file_name.lpstrFilter = "Json Files (*.json)\0*.json\0";
			open_file_name.lpstrFile = file_name;
			open_file_name.nMaxFile = MAX_PATH;
			open_file_name.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			open_file_name.lpstrDefExt = "";

			if (GetOpenFileName(&open_file_name))
			{
				EditorCommand* cmd1 = new EditorCommand();
				cmd1->Type = eCloseLevel;
				gEditorEngine->RunCommand(cmd1);

				EditorCommand_LoadLevel* cmd = new EditorCommand_LoadLevel;
				cmd->Path = String(file_name);
				gEditorEngine->RunCommand(cmd);
			}
		}

		if (ImGui::MenuItem("Close", nullptr))
		{
			EditorCommand* cmd = new EditorCommand();
			cmd->Type = eCloseLevel;
			gEditorEngine->RunCommand(cmd);
		}
		ImGui::EndMenu();
	}
#endif

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

