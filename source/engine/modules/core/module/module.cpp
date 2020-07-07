#include <core/module/module.hpp>

#include <cstring>
#include <cstdio>

// TODO(dark): rewrite to kernel
#include <Windows.h>

struct ModuleLinkedList
{
	IModule* Module;
	void* RawOsHandle;
	ModuleLinkedList* Next;
};

static ModuleLinkedList* Modules = nullptr;

/* We have to keep it as static memory, since we can't use */
/* memory allocation functions as we're not sure if it's initialized. */
static RayModuleEntryFn* StaticallyLoadedModules[32] = {};

ModuleManager::ModuleManager()
{
	for (auto& module : StaticallyLoadedModules)
	{
		if (module == nullptr) 
			continue;
		
		if (Modules == nullptr)
		{
			Modules = new ModuleLinkedList{ (*module)(), nullptr, nullptr };
		}
		else
		{
			ModuleLinkedList* current = Modules;
			ModuleLinkedList* new_ = new ModuleLinkedList{ (*module)(), nullptr, nullptr };

			while (current)
			{
				if (current->Next == nullptr)
				{
					current->Next = new_;
					break;
				}
				current = current->Next;
			}
		}
	}

	memset(&StaticallyLoadedModules, 0, sizeof(RayModuleEntryFn*) * 32);

	/* Call IModule#OnLoad */
	ModuleLinkedList* current = Modules;

	while (current)
	{
		current->Module->OnLoad();
		current = current->Next;
	}
}

Result<IModule*, ModuleLoadError> ModuleManager::LoadModule(pcstr name)
{
	/* 1. Check is it's already loaded. */
	if (Modules != nullptr)
	{
		ModuleLinkedList* current = Modules;

		while (current)
		{
			if (strcmp(current->Module->Meta.Name, name) == 0)
				return { current->Module, eSuccess };
			
			current = current->Next;
		}
	}

	/* Load it. */
	IModule* module = nullptr;
	void* rawHandle = nullptr;

	/* 2. If not in static environment, load the appropriate .dll. */
	if constexpr(RAY_STATIC == 0)
	{
		HMODULE lib = nullptr;
		char tmp[64];
		
		sprintf_s(tmp, "libray-%s.dll", name);
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

	/* 3. Initialize module. */
	module->OnLoad();
	
	/* 4. Add to the linked list. */
	if (Modules == nullptr)
	{
		Modules = new ModuleLinkedList { module, rawHandle, nullptr };
	}
	else
	{
		ModuleLinkedList* current = Modules;
		ModuleLinkedList* new_ = new ModuleLinkedList { module, rawHandle, nullptr };

		while (current)
		{
			if (current->Next == nullptr)
			{
				current->Next = new_;
				break;
			}
			current = current->Next;
		}
	}
	
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

