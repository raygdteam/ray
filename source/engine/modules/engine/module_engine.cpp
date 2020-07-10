#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>
#include <core/module/module.hpp>
#include <engine/state/state.hpp>
#include <core/object/object_db.hpp>

#include <engine/world/world.hpp>

RayObject* Create_World() { return new World; }

struct Engine_Module : public IModule
{
	void OnLoad() override
	{
		this->Meta = ModuleMeta{ "engine", false };

		ObjectDb* objects = RayState()->ObjectDb;

		objects->RegisterObject(new RayObjectMeta{ "//.//world/world", &Create_World });
	}

	void OnUnload() override
	{

	}

	IModuleInterface* QueryModuleInterface() override
	{
		return nullptr;
	}

	~Engine_Module() override
	{

	}
};

REGISTER_MODULE(Engine_Module);