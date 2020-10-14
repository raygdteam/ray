#pragma once

#include "widget.hpp"

class UiManager : public UiWidget
{
public:
	void tick();
	void render();
};