#include "image.hpp"
#include <engine/ui2/ext/imgui.h>

UiImage::UiImage(UiWindow* window) : UiObject(window)
{
	
}

void UiImage::Tick()
{
	ImGui::Image((ImTextureID)1, ImVec2(_parent->GetSize().x, _parent->GetSize().y), ImVec2(0,0), ImVec2(1,1), 
		ImColor(255, 255, 255, 255),
		ImColor(50, 127, 166, 255));
}

