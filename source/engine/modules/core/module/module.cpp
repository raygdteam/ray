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

ModuleLinkedList* _modules = nullptr;

Result<IModule*, ModuleLoadError> ModuleManager::LoadModule(pcstr name)
{
	/* 1. Check is it's already loaded. */
	if (_modules != nullptr)
	{
		ModuleLinkedList* current = _modules;

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

		using RayModuleEntry = IModule*();

		RayModuleEntry* entry = reinterpret_cast<RayModuleEntry*>(GetProcAddress(lib, "RayModuleEntry"));
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
	if (_modules == nullptr)
	{
		_modules = new ModuleLinkedList { module, rawHandle, nullptr };
	}
	else
	{
		ModuleLinkedList* current = _modules;
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

// TODO(dark): rewrite it to some kind of GlobalState
RAY_CORE_API ModuleManager* gModuleManager = new ModuleManager();
