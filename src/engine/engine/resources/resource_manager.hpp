#pragma once
#include "engine/engine.hpp"
#include "resource.hpp"

namespace ray::resources
{

class RAY_ENGINE_API ResourceManager
{
	static ResourceManager* __Instance;
};

}