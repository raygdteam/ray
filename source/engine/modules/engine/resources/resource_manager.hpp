#pragma once
#include <core/lib/delegate.hpp>
#include <core/lib/string.hpp>
#include <core/threading/critical_section.hpp>
#include <engine/resources/resource.hpp>


// Internal use only;
struct ResourceData;
struct ResourceMapping;

class RAY_ENGINE_API ResourceManager
{
	ray::CriticalSection _mutex;
	Array<ResourceData> _resources;
	Array<ResourceMapping> _mapping;

	/* data cache */
	String _dataCacheDirectory;

	IRResource* LoadResourceResolved(pcstr path, pcstr resorcePath, ResourceType type);
public:
	ResourceManager();

	void SetResourceDirectory(pcstr directory, pcstr mapping = "");

	IRResource* LoadResourceSync(pcstr name, ResourceType desiredType);

	void LoadResource(pcstr name, ResourceType desiredType, Function<void(IRResource*)> callback);
};
