#pragma once
#include <engine/ui2/ui2.hpp>

class EdLogWindow : public UiWindow
{
public:
	EdLogWindow();
	void Tick() override;
};
