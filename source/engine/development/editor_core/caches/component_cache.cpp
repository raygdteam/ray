#pragma once
#include <engine/state/state.hpp>
#include <engine/world/components/component_base.hpp>
#include "component_cache.hpp"

ComponentCache::ComponentCache() : _log("ComponentCache")
{
}

void ComponentCache::Rebuild()
{
	Array<Type*>& types = RayState()->ObjectDb->GetAllTypes();

	for (Type* type : types)
	{
		if (type->Abstract) continue;

		RayObject* obj = type->CreateInstance();

		if (dynamic_cast<IComponent*>(obj) != nullptr)
		{
			_cache.PushBack(type);
		}

		delete obj;
	}

	_log.Log("Cached %llu objects out of %llu total.", _cache.Size(), types.Size());
}

Array<Type*>& ComponentCache::GetCache()
{
	return _cache;
}

ComponentCache* gComponentCache = nullptr;
