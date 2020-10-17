#pragma once
#include <engine/ui/object.hpp>

class UiButton : public UiObject
{
protected:
	virtual void OnPress() = 0;
public:
	void Tick() override;
	UiButton();
};
