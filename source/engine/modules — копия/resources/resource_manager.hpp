#pragma once
#include <core/lib/delegate.hpp>
#include <core/lib/string.hpp>
#include <core/threading/critical_section.hpp>
#include <resources/resource.hpp>


struct IRayState;

// Internal use only;
struct ResourceData;
struct ResourceMapping;

class RAY_RESOURCES_API ResourceManager
{
	IRayState* _state;
	
	CriticalSection _mutex;
	Array<ResourceData> _resources;
	Array<ResourceMapping> _mapping;

	/* data cache */
	String _dataCacheDirectory;

	IRResource* LoadResourceResolved(pcstr path, pcstr resorcePath, ResourceType type);
public:
	ResourceManager(IRayState* state);

	void SetResourceDirectory(pcstr directory, pcstr mapping = "");

	IRResource* LoadResourceSync(pcstr name, ResourceType desiredType);

	void LoadResource(pcstr name, ResourceType desiredType, Function<void(IRResource*)> callback);
};
