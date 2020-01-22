#pragma once
#include <pch.h>
#include <cstdlib>

namespace ray::component
{

struct IComponentBase
{
	virtual ~IComponentBase() = default;


	virtual void BeginPlay() = 0;
	virtual void Tick(float delta) = 0;
	virtual void Destroy() = 0;

	// The transformation is calculated relative to parent acrot.
	virtual glm::vec3 GetPosition() = 0;
	virtual glm::vec3 GetRotation() = 0;
	virtual glm::vec3 GetScale() = 0;
};

struct IComponent : public IComponentBase
{
	~IComponent() override {}
	void BeginPlay() override {}
	void Tick(float delta) override {}
	void Destroy() override {}

	// The transformation is calculated relative to parent acrot.
	glm::vec3 GetPosition() override { return _position; }
	glm::vec3 GetRotation() override { return _rotation; }
	glm::vec3 GetScale() override { return _scale; }
protected:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
};




}