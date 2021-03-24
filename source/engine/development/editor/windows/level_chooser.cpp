#include "level_chooser.hpp"


#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"

LevelChooser::LevelChooser()
{
	Title = String("Choose Level");

	bCenterOnFirstTime = true;
}

void LevelChooser::Tick()
{
	ImGui::TextUnformatted("Choose a level to load");
	
	if (ImGui::Button("Level1"))
	{
		EditorCommand_LoadLevel* cmd = new EditorCommand_LoadLevel;
		cmd->Path = String("../../engine/test_project1/resources/levels/level.json");
		gEditorEngine->RunCommand(cmd);
	}

	if (ImGui::Button("Level2"))
	{
		EditorCommand_LoadLevel* cmd = new EditorCommand_LoadLevel;
		cmd->Path = String("../../engine/resources/level2.json");
		gEditorEngine->RunCommand(cmd);
	}
}
