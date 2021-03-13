#pragma once
#include <engine/engine/engine_def.hpp>
#include <core/lib/string.hpp>
#include <engine/ui2/ui2.hpp>

class RAY_ENGINE_API UiLabel : public UiObject
{
	String& _text;
public:
	UiLabel(UiWindow* window, String& text);
	
	void Tick() override;
};
