#include "level_viewport.hpp"


#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/ui2/objects/image.hpp"
#include "renderer_core/resources/buffer_manager.hpp"

EdLevelViewport::EdLevelViewport()
{
	Title = String("Level Viewport");
	//this->_objects.PushBack(new UiImage(this));
	Padding = { 4.f, 0.f };
	bNoScrollbar = true;
}

void EdLevelViewport::Tick()
{
	if (!gEditorEngine->IsCurrentlyInLevel()) return;

	static u32 oldWidth;
	static u32 oldHeight;
	
	u32 width = static_cast<u32>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	u32 height = static_cast<u32>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

	if ((width != oldWidth || height != oldHeight) && width < 10000 && height < 10000)
	{
		gSceneColorBuffer.Reset(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, "gSceneColorBuffer");
		
		oldWidth = width;
		oldHeight = height;

		gEditorEngine->ResizeCameraViewport({ f32(width), f32(height) });
	}

	ImGui::Image((ImTextureID)1, ImVec2(width, height), ImVec2(0, 0), ImVec2(1, 1),
		ImColor(255, 255, 255, 255),
		ImColor(50, 127, 166, 255));
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

