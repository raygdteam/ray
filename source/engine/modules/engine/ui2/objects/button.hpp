#pragma once
#include <core/lib/string.hpp>
#include <engine/ui2/ui2.hpp>

class UiButton : public UiObject
{
	String _text;
public:
	UiButton(UiWindow* window, String& text);
	void Tick() override;
};
