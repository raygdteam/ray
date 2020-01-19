#pragma once
#include <pch.h>
#include "component/IComponent.hpp"

namespace ray
{

struct IActorBase
{
	virtual ~IActorBase() = default;

	
	virtual void BeginPlay() = 0;
	virtual void Tick() = 0;
	virtual void Destroy() = 0;

	virtual glm::vec3 GetPosition() = 0; // for 2d z = 0
	virtual glm::vec3 GetRotation() = 0; // for 2d z = 0
	virtual glm::vec3 GetScale() = 0; // for 2d z = 0
};

struct IActorUtility : public IActorBase
{
protected:
	std::vector<component::IComponent> _components;

	template<class T, class... Args>
	inline void add_component(Args&&... args)
	{
		_components.push_back(std::make_shared<T>(args));
	}
};


class IActor : public IActorUtility
{
	glm::vec3 GetPosition() override { return _position; }
	glm::vec3 GetRotation() override { return _rotation; }
	glm::vec3 GetScale() override { return _scale; }
protected:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
};

}