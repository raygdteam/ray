#include <engine/ui2/objects/button.hpp>
#include <engine/ui2/ext/imgui.h>

UiButton::UiButton(String& text)
{
	_text = text;
}

void UiButton::Tick()
{
	ImGui::Button(_text.AsRawStr());
}

