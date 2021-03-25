#pragma once
#include <engine/ui2/ui2.hpp>
#include <resources/resource.hpp>

class EdResourceBrowser : public UiWindow
{
public:
	EdResourceBrowser();
	void Tick() override;
};
