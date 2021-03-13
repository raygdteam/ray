#include <engine/ui2/objects/button.hpp>
#include <engine/ui2/ext/imgui.h>

UiButton::UiButton(UiWindow* window, String& text) : UiObject(window)
{
	_text = text;
}

void UiButton::Tick()
{
	ImGui::Button(_text.AsRawStr());
}

