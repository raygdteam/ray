#include "debug_window.hpp"


#include "editor/engine/engine.hpp"
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

void EdDebugWindow::Tick()
{
	gText.clear();

	size_t count = IRenderer::sRendererStats.DrawCallsCount;
	gText.append_sprintf("DrawCallsCount = %i", count);

	FVector2& pos = gEditorEngine->GetCameraPos();
	float* posx = (float*)&pos.x;
	
	if (ImGui::SliderFloat2("Camera pos", posx, -1000.f, 1000.f))
	{
		gEditorEngine->ApplyMouseDragOnViewport(FVector2{ 0,0 });
	}

	ImGui::SliderFloat("Sensitivity", &gEditorEngine->MouseDragSensitivity, 1.f, 10.f);

	memcpy(&gFrametimeFrames[1], &gFrametimeFrames[0], 255 * sizeof(f32));
	gFrametimeFrames[0] = gEditorEngine->GetDelta();
	f32 avg = 0;
	for (f32 frametimeFrame : gFrametimeFrames)
		avg += frametimeFrame;
	avg /= 256;
	ImGui::PlotHistogram("Frametime dt", &gFrametimeFrames[0], 256, 0,0,avg - 10.f, avg + 10.f, ImVec2(0, 80.0f));
}

