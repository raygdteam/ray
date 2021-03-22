#include <engine/ui2/objects/label.hpp>
#include <engine/ui2/ext/imgui.h>

UiLabel::UiLabel(UiWindow* window, String& text) : UiObject(window), _text(text)
{
}

void UiLabel::Tick()
{
	ImGui::TextUnformatted(_text.AsRawStr());
}

