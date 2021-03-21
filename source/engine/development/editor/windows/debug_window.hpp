#pragma once
#include <engine/ui2/ui2.hpp>

class EdDebugWindow : public UiWindow
{
public:
	EdDebugWindow();
	void Tick() override;
};

extern bool gShowDemoWindow;

