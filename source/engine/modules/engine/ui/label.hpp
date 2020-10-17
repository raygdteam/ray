#pragma once
#include <core/lib/string.hpp>
#include <engine/ui/object.hpp>


class UiLabel : public UiObject
{
	String _text;
protected:
	void SetText(String& text);
	void SetText(String text);
public:
	UiLabel();
	UiLabel(String text);
	UiLabel(const char* text);
};
