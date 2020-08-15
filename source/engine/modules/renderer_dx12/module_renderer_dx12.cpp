#include "pch.hpp"
#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>
#include <core/module/module.hpp>

#include <renderer_core/ray_renderer_core_class_helper.hpp>



class D3D12ModuleInterface : public ray::renderer_core_api::IRRCClassHelper
{
public:
	
};

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
		static D3D12ModuleInterface interface_;
		return &interface_;
	}
	
	~Renderer_Dx12_Module() override
	{
		
	}
};

REGISTER_MODULE(Renderer_Dx12_Module);

#ifdef RAY_RELEASE
int foo(const int bar)
{
	init_res = bar >> 1;
	return init_res;
}
#endif
