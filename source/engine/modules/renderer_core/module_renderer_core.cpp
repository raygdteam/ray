#include <core/memory/new_delete_override.hpp>
#include <core/core.hpp>
#include <core/module/module.hpp>

struct Renderer_Core_Module : public IModule
{
	void OnLoad() override
	{
		this->Meta = ModuleMeta{ "renderer_dx12", false };
	}

	void OnUnload() override
	{

	}

	IModuleInterface* QueryModuleInterface() override
	{
		return nullptr;
	}

	~Renderer_Core_Module() override
	{

	}
};

REGISTER_MODULE(Renderer_Core_Module);