#pragma once
#include "GLM/glm.hpp"
#include "core/object/object.hpp"

namespace ray
{

class Actor;

struct Component : public Object
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Actor* owner;

	string GetClassName() override { return "<UNKNOWN COMPONENT>"; }
};

struct ModelComponent : Component
{
	pcstr material;
	pcstr model;
};

class Actor : public Object
{
	std::vector<Component*> components;
public:
	Actor();

	virtual void BeginPlay() = 0;
	virtual void Tick(float) = 0;
	virtual void Destroy() = 0;

	string GetClassName() override { return "<UNKNOWN ACTOR>"; }
};

}
