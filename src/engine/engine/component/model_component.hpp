#pragma once
#include "IComponent.hpp"
#include "world/Actor.hpp"

namespace ray::component
{

class ModelComponent : public IComponent
{
	std::string _modelName;
public:
	ModelComponent(pcstr modelName) : IComponent(), _modelName(modelName) 
	{}
};

class ModelComponentActor : public ray::IActor
{
public:
	ModelComponentActor(pcstr model) : IActor()
	{
		add_component<ModelComponent>(model);
	}
};

}
