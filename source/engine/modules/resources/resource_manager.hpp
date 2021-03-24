#pragma once
#include <core/lib/delegate.hpp>
#include <core/lib/string.hpp>
#include <core/threading/critical_section.hpp>
#include <resources/resource.hpp>


struct IRayState;

// Internal use only;
struct ResourceData;

class RAY_RESOURCES_API ResourceManager
{
	struct ResourceMapping
	{
		String Path;
		String Mapping;
	};
	
	IRayState* _state;
	
	CriticalSection _mutex;
	Array<ResourceData> _resources;
	
	ResourceMapping _engineMap;
	ResourceMapping _gameMap;

	/* data cache */
	String _dataCacheDirectory;

	IRResource* LoadResourceResolved(pcstr path, pcstr resorcePath, ResourceType type);
public:
	ResourceManager(IRayState* state);

	void SetEngineResourcesDirectory(String& directory);
	void SetGameResourceDirectory(String& directory);
	void UnloadAllResources();
	
	IRResource* LoadResourceSync(pcstr name, ResourceType desiredType);

	void LoadResource(pcstr name, ResourceType desiredType, Function<void(IRResource*)> callback);
};
