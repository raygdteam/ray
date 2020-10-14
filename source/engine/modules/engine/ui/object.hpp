#pragma once
#include <core/core.hpp>

#include <engine/world/components/transform.hpp>

class UiObject
{
	friend class UiWidget;
protected:
	Transform* _transform;
public:
	UiObject();
	virtual ~UiObject() {};

	virtual void Tick() = 0;
};

class UiButton : public UiObject
{
protected:
	virtual void OnPress() = 0;
};
