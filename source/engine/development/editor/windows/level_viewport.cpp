#include "level_viewport.hpp"


#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/ui2/objects/image.hpp"

EdLevelViewport::EdLevelViewport()
{
	Title = String("Level Viewport");
	this->_objects.PushBack(new UiImage(this));
	Padding = { 4.f, 0.f };
	bNoScrollbar = true;
}

void EdLevelViewport::Tick()
{
	//float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	//float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
}

void EdLevelViewport::LateTick()
{
	if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right))
	{
		ImGuiIO& io = ImGui::GetIO();
		gEditorEngine->ApplyMouseDragOnViewport({ io.MouseDelta.x, io.MouseDelta.y });
	}
	
	if (ImGui::IsItemHovered())
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.MouseWheel != 0.0f)
		{
			gEditorEngine->ApplyMouseZoomOnViewport(io.MouseWheel);
		}
	}
}

