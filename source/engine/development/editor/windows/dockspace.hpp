#pragma once
#include <engine/ui2/ui2.hpp>

class EdDockspace : public UiDockspace
{
	void MenuBar();
public:
	void Tick() override;
};
