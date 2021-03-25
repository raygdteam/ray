#pragma once
#include <core/lib/delegate.hpp>
#include <core/lib/string.hpp>
#include <core/threading/critical_section.hpp>
#include <resources/resource.hpp>


struct IRayState;

// Internal use only;
struct ResourceData
{
	String Name;
	String PhysicalPath;
	u16 ReferenceCount;
	ResourceType Type;
	IRResource* ResourceRef;
};

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

	void LoadPreloadedResource(ResourceData& data);
	IRResource* LoadResourceResolved(pcstr path, pcstr resorcePath, ResourceType type);
	void PreloadResource(String& path, String& resourcePath);
public:
	ResourceManager(IRayState* state);

	void SetEngineResourcesDirectory(String& directory);
	void SetGameResourceDirectory(String& directory);
	void UnloadAllResources();
	
	IRResource* LoadResourceSync(pcstr name, ResourceType desiredType);

	void LoadResource(pcstr name, ResourceType desiredType, Function<void(IRResource*)> callback);

	void PreloadAllEngineResources();
	void PreloadAllGameResources();

	Array<ResourceData>& GetAllResources()
	{
		return _resources;
	}
};
