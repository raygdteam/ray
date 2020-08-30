#include <core/core.hpp>
#include <core/module/module.hpp>
#include <core/lib/array.hpp>

#include <cstring>
#include <cstdio>

// TODO(dark): rewrite to kernel
#include <Windows.h>

struct ModuleDef
{
	IModule* Module;
	void* RawOsHandle;
};

static Array<ModuleDef*>* gModules = nullptr;

/* We have to keep it as static memory, since we can't use */
/* memory allocation functions as we're not sure if it's initialized. */
static RayModuleEntryFn* StaticallyLoadedModules[32] = {};

ModuleManager::ModuleManager()
{
	if (gModules == nullptr)
		gModules = new Array<ModuleDef*>();
	else __debugbreak();

	for (auto& module : StaticallyLoadedModules)
	{
		if (module == nullptr)
			continue;

		gModules->push_back(new ModuleDef{ (*module)(), nullptr });
	}

	memset(&StaticallyLoadedModules, 0, sizeof(RayModuleEntryFn*) * 32);

	/* Call IModule#OnLoad */
	for (ModuleDef*& gModule : *gModules)
	{
		gModule->Module->OnLoad();
	}
}

Result<IModule*, ModuleLoadError> ModuleManager::LoadModule(pcstr name)
{
	/* 1. Check is it's already loaded. */
	if (!gModules->empty())
	{
		for (ModuleDef* module : *gModules)
		{
			if (strcmp(module->Module->Meta.Name, name) == 0)
				return { module->Module, eSuccess };
		}
	}

	/* Load it. */
	IModule* module = nullptr;
	void* rawHandle = nullptr;

	/* 2. If not in static environment, load the appropriate .dll. */
#if RAY_STATIC == 0
	{
		HMODULE lib = nullptr;
		char tmp[64];

		sprintf_s(tmp, "{}.dll", name);
		lib = LoadLibraryA(tmp);
		if (lib == nullptr)
		{
			return { nullptr, eOsError };
		}

		RayModuleEntryFn* entry = reinterpret_cast<RayModuleEntryFn*>(GetProcAddress(lib, "RayModuleEntry"));
		if (entry == nullptr)
		{
			FreeLibrary(lib);
			return { nullptr,eNotARayModule };
		}

		module = (*entry)();
		rawHandle = lib;
	}
#endif
	/* 3. Initialize module. */
	module->OnLoad();

	/* 4. Add to the linked list. */
	gModules->push_back(new ModuleDef{ module, rawHandle });

	return { module, eSuccess };
}

void ModuleManager::__Internal_LoadModuleStatic(RayModuleEntryFn module)
{
	for (u8 i = 0; i < 32; ++i)
	{
		if (StaticallyLoadedModules[i] == nullptr)
		{
			StaticallyLoadedModules[i] = module;
			return;
		}
	}
}
