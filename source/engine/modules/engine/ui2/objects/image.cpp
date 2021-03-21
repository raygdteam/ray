#include "image.hpp"
#include <engine/ui2/ext/imgui.h>

UiImage::UiImage(UiWindow* window) : UiObject(window)
{
	
}

void UiImage::Tick()
{
	float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
	
	ImGui::Image((ImTextureID)1, ImVec2(width, height), ImVec2(0,0), ImVec2(1,1), 
		ImColor(255, 255, 255, 255),
		ImColor(50, 127, 166, 255));
}

