#pragma once
#include "pch.h"
#include "component/IComponent.hpp"

namespace ray
{

struct IActorBase
{
	virtual ~IActorBase() = default;

	
	virtual void BeginPlay() = 0;
	virtual void Tick(float delta) = 0;
	virtual void Destroy() = 0;

	virtual glm::vec3 GetPosition() = 0;
	virtual glm::vec3 GetRotation() = 0;
	virtual glm::vec3 GetScale() = 0;
};

struct IActorUtility : public IActorBase
{
protected:
	std::vector<component::IComponent> _components;

	template<class T, class... Args>
	inline void add_component(Args&&... args)
	{
		_components.push_back(T(args...));
	}
};


class IActor : public IActorUtility
{
public:
	IActor() { _position = glm::vec3(0.f, 0.f, 0.f); _scale = glm::vec3(1.f, 1.f, 1.f); }

	void BeginPlay() override {}
	void Tick(float delta) override {}
	void Destroy() override {}

	glm::vec3 GetPosition() override { return _position; }
	glm::vec3 GetRotation() override { return _rotation; }
	glm::vec3 GetScale() override { return _scale; }

	std::vector<component::IComponent> GetComponents()
	{ return _components; }
protected:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
};

}