#pragma once
#include <core/core.hpp>
#include <core/module/module_meta.hpp>
#include <core/lib/result.hpp>

namespace ray
{
enum ModuleLoadError : u8
{
	eSuccess = 0,
	eOsError,
	eNotARayModule,
};

using RayModuleEntryFn = IModule * ();

class RAY_CORE_API ModuleManager
{
public:
	ModuleManager();

	Result<IModule*, ModuleLoadError> LoadModule(pcstr name);


	static void __Internal_LoadModuleStatic(RayModuleEntryFn);
};
}

/* In static environment, just call the __Internal_LoadModuleStatic() */
#if RAY_STATIC == 1
#define REGISTER_MODULE(Module) extern "C" static IModule* RayModuleEntry() { static IModule* instance = nullptr; if (instance == nullptr) instance = new Module(); return instance; } \
	static int init() { ray::ModuleManager::__Internal_LoadModuleStatic(RayModuleEntry); return 0; } static int init_res = init();
#else /* In dynamic, export a function that returns a singleton of a Module. */
#define REGISTER_MODULE(Module) extern "C" RAY_DLLEXPORTS IModule* RayModuleEntry() { static IModule* instance = nullptr; if (instance == nullptr) instance = new Module(); return instance; }
#endif
