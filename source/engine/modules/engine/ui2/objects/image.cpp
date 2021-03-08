#include "image.hpp"
#include <engine/ui2/ext/imgui.h>

void UiImage::Tick()
{
	ImGui::Image((ImTextureID)1, ImVec2(100.f, 100.f));
}

