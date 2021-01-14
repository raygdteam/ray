#include <engine/ui2/objects/label.hpp>
#include <engine/ui2/ext/imgui.h>

UiLabel::UiLabel(String& text)
{
	_text = text;
}

void UiLabel::Tick()
{
	ImGui::Text("%s", _text.AsRawStr());
}

