#pragma once
#include <engine/state/state.hpp>
#include <engine/world/actor.hpp>
#include "actor_cache.hpp"


struct ActorCache::ActorCacheEntry
{
	Type* Type;
};

ActorCache::ActorCache() : _log("ActorCache")
{
}

void ActorCache::Rebuild()
{
	Array<Type*>& types = RayState()->ObjectDb->GetAllTypes();

	for (Type* type : types)
	{
		if (type->Abstract) continue;

		RayObject* obj = type->CreateInstance();

		if (dynamic_cast<Actor*>(obj) != nullptr)
		{
			_cache.PushBack(ActorCacheEntry{ type });
		}

		delete obj;
	}

	_log.Log("Cached %llu objects out of %llu total.", _cache.Size(), types.Size());
}

Array<ActorCache::ActorCacheEntry>& ActorCache::GetCache()
{
	return _cache;
}

ActorCache* gActorCache = nullptr;
