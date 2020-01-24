#pragma once
#include "pch.h"
#include "IComponent.hpp"
#include "world/Actor.hpp"

namespace ray::component
{

class CameraComponent : public IComponent
{
	void updateViewMatrix();
public:
	CameraComponent() : IComponent()
	{}

	struct
	{
		glm::mat4 perspective;
		glm::mat4 view;
	} matrices;


	void Tick(float delta) override;
};

class CameraComponentActor : public ray::IActor
{
public:
	CameraComponentActor() : IActor()
	{
		add_component<CameraComponent>();
	}


	void Tick(float delta) override;
};

}
