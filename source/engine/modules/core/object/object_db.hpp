#pragma once
#include <core/core.hpp>
#include <core/object/object.hpp>

class RAY_CORE_API ObjectDb
{
public:
	ObjectDb();
	
	RayObjectMeta* GetObject(pcstr name);
	void RegisterObject(RayObjectMeta*);
};
