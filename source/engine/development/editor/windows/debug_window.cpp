#include "debug_window.hpp"


#include "editor/engine/engine.hpp"
#include "engine/state/state.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/ui2/objects/label.hpp"
#include "renderer_core/renderer.hpp"

String gText("AAAAAAAAAAAAAAAAAAAA");

EdDebugWindow::EdDebugWindow()
{
	Title = String("EdDebugWindow");
	
	this->_objects.PushBack(new UiLabel(this, gText));
}

f32 gFrametimeFrames[256];
bool gShowDemoWindow = false;

void EdDebugWindow::Tick()
{
	gText.clear();

	size_t count = IRenderer::sRendererStats.LastFrameDC;
	gText.append_sprintf("DrawCallsCount = %i\n", count);
	gText.append_sprintf("AllocatedVirtualMemory = %i\n", IRenderer::sRendererStats.AllocatedVirtualMemory);
	gText.append_sprintf("UsedVirtualMemory = %llu", IRenderer::sRendererStats.UsedVirtualMemory);

	if (gEditorEngine->IsCurrentlyInLevel())
	{
		FVector2& pos = gEditorEngine->GetCameraPos();
		float* posx = (float*)&pos.x;

		if (ImGui::SliderFloat2("Camera pos", posx, -1000.f, 1000.f))
		{
			gEditorEngine->ApplyMouseDragOnViewport(FVector2{ 0,0 });
		}

		if (ImGui::DragFloat("Camera zoom", gEditorEngine->GetCameraZoom(), 0.025f))
		{
			gEditorEngine->ApplyMouseDragOnViewport(FVector2{ 0,0 });
		}
	}
	
	ImGui::SliderFloat("Sensitivity", &gEditorEngine->MouseDragSensitivity, 1.f, 10.f);

	memcpy(&gFrametimeFrames[1], &gFrametimeFrames[0], 255 * sizeof(f32));
	gFrametimeFrames[0] = gEditorEngine->GetDelta();
	f32 avg = 0;
	for (f32 frametimeFrame : gFrametimeFrames)
		avg += frametimeFrame;
	avg /= 256;
	ImGui::PlotHistogram("Frametime dt", &gFrametimeFrames[0], 256, 0,0,avg - 10.f, avg + 10.f, ImVec2(0, 80.0f));
	ImGui::Text("Delta = %f", gEditorEngine->GetDelta());
	if (ImGui::Button("Load Level1"))
	{
		EditorCommand* cmd1 = new EditorCommand();
		cmd1->Type = eCloseLevel;
		gEditorEngine->RunCommand(cmd1);
		
		EditorCommand_LoadLevel* cmd = new EditorCommand_LoadLevel;
		cmd->Path = String("../../engine/test_project1/resources/levels/level.json");
		gEditorEngine->RunCommand(cmd);
	}
	
	if (ImGui::Button("Load Level2"))
	{
		EditorCommand* cmd1 = new EditorCommand();
		cmd1->Type = eCloseLevel;
		gEditorEngine->RunCommand(cmd1);
		
		EditorCommand_LoadLevel* cmd = new EditorCommand_LoadLevel;
		cmd->Path = String("../../engine/resources/level2.json");
		gEditorEngine->RunCommand(cmd);
	}

	ImGui::Checkbox("Show Demo Window", &gShowDemoWindow);

	if (ImGui::Button("Close project"))
	{
		EditorCommand* cmd = new EditorCommand;
		cmd->Type = eCloseLevel;
		gEditorEngine->RunCommand(cmd);
	}
	
	ImGui::Separator();
	ImGui::Spacing();

	Array<RayObject*>& objs = RayState()->ObjectDb->GetAllObjects();

	ImGui::Text("%llu objects:", objs.Size());
	
	for (RayObject* obj : objs)
	{
		ImGui::TextUnformatted(obj->GetType()->Name);
	}
	
	ImGui::Separator();
	ImGui::Spacing();
}

