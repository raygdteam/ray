#pragma once
#include <core/core.hpp>
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <core/log/log.hpp>

class ComponentCache
{
	struct ComponentCacheEntry;
	Array<ComponentCacheEntry> _cache;
	Logger _log;
	
public:
	ComponentCache();
	void Rebuild();

	Array<ComponentCacheEntry>& GetCache();
};

extern ComponentCache* gComponentCache;
