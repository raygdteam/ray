#include <engine/ui/label.hpp>


void UiLabel::SetText(String& text)
{
	_text = text;
}

void UiLabel::SetText(String text)
{
	_text = text;
}

UiLabel::UiLabel()
{
	_text = String("");
}

UiLabel::UiLabel(String text)
{
	_text = text;
}

UiLabel::UiLabel(const char* text)
{
	_text = String(text);
}
