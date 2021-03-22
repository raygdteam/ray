#pragma once
#include <core/core.hpp>
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <core/log/log.hpp>

class ComponentCache
{
	Array<Type*> _cache;
	Logger _log;

public:
	ComponentCache();
	void Rebuild();

	Array<Type*>& GetCache();
};

extern ComponentCache* gComponentCache;
