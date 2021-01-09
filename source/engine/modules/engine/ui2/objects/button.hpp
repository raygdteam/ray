#pragma once
#include <core/lib/string.hpp>
#include <engine/ui2/ui2.hpp>

class UiButton : public UiObject
{
	String _text;
public:
	UiButton(String& text);
	void Tick() override;
};
