#pragma once
#include <core/core.hpp>
#include <core/object/object.hpp>

class Actor;

class Component : public RayObject
{
	RAYOBJECT_BODY(Component, RayObject);
protected:
	Actor* _parent = nullptr;
public:
	virtual void Tick() = 0;
};
