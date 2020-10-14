#pragma once

#include "widget.hpp"

class UIManager : public UIWidget
{
public:
	void Tick();
	void Render();
};