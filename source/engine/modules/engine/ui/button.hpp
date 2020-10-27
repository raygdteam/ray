#pragma once
#include <engine/ui/object.hpp>

class UiButton : public UiObject
{
protected:
	virtual void OnPress() = 0;
public:
	UiButton();
	
	void Tick() override;
	void Render() override;
};
