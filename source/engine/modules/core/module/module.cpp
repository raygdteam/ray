#include <core/core.hpp>
#include <core/module/module.hpp>
#include <core/lib/array.hpp>
#include <core/log/log.hpp>

#include <cstring>
#include <cstdio>

// TODO(dark): rewrite to kernel
#include <Windows.h>

static Logger* gMMLog;

/* We have to keep it as static memory, since we can't use */
/* memory allocation functions as we're not sure if it's initialized. */
static RayModuleEntryFn* StaticallyLoadedModules[32] = {};

ModuleManager::ModuleManager()
{
	gMMLog = new Logger("ModuleManager");

	for (auto& mod : StaticallyLoadedModules)
	{
		if (mod == nullptr)
			continue;

		_modules.PushBack(ModuleDef { (*mod)(), nullptr });
	}

	gMMLog->Log("Discovered %ull statically linked modules", _modules.Size());
	
	memset(&StaticallyLoadedModules, 0, sizeof(RayModuleEntryFn*) * 32);

	/* Call IModule#OnLoad */
	for (ModuleDef& gModule : _modules)
	{
		gModule.Module->OnLoad();
	}
}

ModuleManager::~ModuleManager()
{
	for (ModuleDef& mod : _modules)
	{
		gMMLog->Log("Shutting down and freeing module %s.", mod.Module->Meta.Name);
		mod.Module->OnUnload();

		if (mod.RawOsHandle != nullptr) 
			FreeLibrary(static_cast<HMODULE>(mod.RawOsHandle));
	}

	_modules.clear();
	gMMLog->Log("Bye.");
	delete gMMLog;
}

Result<IModule*, ModuleLoadError> ModuleManager::LoadModule(pcstr name)
{
	/* Check is it's already loaded. */
	for (ModuleDef& mod : _modules)
	{
		if (strcmp(mod.Module->Meta.Name, name) == 0)
			return { mod.Module, eSuccess };
	}

	/* Load it. */
	IModule* mod = nullptr;
	void* rawHandle = nullptr;

	/* If not in static environment, load the appropriate .dll. */
#if RAY_STATIC == 0
	{
		HMODULE lib = nullptr;
		char tmp[64];

		sprintf_s(tmp, "%s.dll", name);
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

		mod = (*entry)();
		rawHandle = lib;
	}
#endif
	/* 3. Initialize module. */
	mod->OnLoad();

	/* 4. Add to the linked list. */
	_modules.push_back(ModuleDef { mod, rawHandle });

	return { mod, eSuccess };
}

void ModuleManager::__Internal_LoadModuleStatic(RayModuleEntryFn mod)
{
	for (u8 i = 0; i < 32; ++i)
	{
		if (StaticallyLoadedModules[i] == nullptr)
		{
			StaticallyLoadedModules[i] = mod;
			return;
		}
	}
}
