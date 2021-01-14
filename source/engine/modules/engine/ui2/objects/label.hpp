#pragma once
#include <core/lib/string.hpp>
#include <engine/ui2/ui2.hpp>

class UiLabel : public UiObject
{
	String _text;
public:
	UiLabel(String& text);
	void Tick() override;
};
