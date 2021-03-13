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
}

