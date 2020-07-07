#include "pch.hpp"
#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>
#include <core/module/module.hpp>

struct Renderer_Dx12_Module : public IModule
{
	void OnLoad() override
	{
		this->Meta = ModuleMeta { "renderer_dx12", false };
	}
	
	void OnUnload() override
	{
		
	}
	
	IModuleInterface* QueryModuleInterface() override
	{
		return nullptr;
	}
	
	~Renderer_Dx12_Module() override
	{
		
	}
};

extern "C" RAY_DLLEXPORTS IModule* RayModuleEntry()
{
	static IModule* instance = nullptr;
	if (instance == nullptr) instance = new Renderer_Dx12_Module();
	return instance;
}
