#pragma once
#include <engine/resources/resource.hpp>
#include <core/lib/delegate.hpp>

// Internal use only;
struct ResourceData;
struct ResourceMapping;

class RAY_ENGINE_API ResourceManager
{
	Array<ResourceData> _resources;
	Array<ResourceMapping> _mapping;

	IRResource* LoadResourceResolved(pcstr path, ResourceType type);
public:
	ResourceManager();

	void SetResourceDirectory(pcstr directory, pcstr mapping = "");

	IRResource* LoadResourceSync(pcstr name, ResourceType desiredType);

	void LoadResource(pcstr name, ResourceType desiredType, Function<void(IRResource*)> callback);
};
