#pragma once
#include "imodel_component.hpp"
#include "actor.hpp"

namespace ray
{

class Level
{
public:
	Level();
	~Level();

	void OnUpdate();

	std::vector<std::shared_ptr<ray::Actor>> GetActors()
	{
		return _actors;
	}
private:
	std::vector<std::shared_ptr<ray::Actor>> _actors;
};

}