#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>

#include "core/module/module.hpp"

struct Engine_Module : public IModule
{
	void OnLoad() override
	{
		this->Meta = ModuleMeta { "engine", false };
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
