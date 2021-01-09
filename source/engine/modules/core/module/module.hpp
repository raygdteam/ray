#pragma once
#include <core/core.hpp>
#include <core/module/module_meta.hpp>
#include <core/lib/result.hpp>
#include <core/lib/array.hpp>
#include <core/log/log.hpp>

enum ModuleLoadError : u8
{
	eSuccess = 0,
	eOsError,
	eNotARayModule,
};

/**
 * Used internally by ModuleManager to keep track of module data.
 * @hide
 */
struct ModuleDef
{
	IModule* Module;
	void* RawOsHandle;
};

/**
 * Delegate to create the module object.
 */
using RayModuleEntryFn = IModule * ();

/**
 * Primary class for managing modules.
 *
 * @note Keep only one instance of this class.
 */
class RAY_CORE_API ModuleManager
{
	Array<ModuleDef> _modules;
	Logger _log;
	
public:
	ModuleManager();
	~ModuleManager();

	/**
	 * Primary method of getting/loading modules.
	 * If the module is already loaded, returns it's object.
	 * @param name The name of module to load.
	 */
	Result<IModule*, ModuleLoadError> LoadModule(pcstr name);

	/**
	 * @hide
	 */
	static void __Internal_LoadModuleStatic(RayModuleEntryFn);
};

/* In static environment, just call the __Internal_LoadModuleStatic() */
#if RAY_STATIC == 1
#define REGISTER_MODULE(Module) static IModule* RayModuleEntry() { static IModule* instance = nullptr; if (instance == nullptr) instance = new Module(); return instance; } \
	static int init() { ModuleManager::__Internal_LoadModuleStatic(RayModuleEntry); return 0; } static int init_res = init();
#else /* In dynamic, export a function that returns a singleton of a Module. */
#define REGISTER_MODULE(Module) extern "C" RAY_DLLEXPORTS IModule* RayModuleEntry() { static IModule* instance = nullptr; if (instance == nullptr) instance = new Module(); return instance; }
#endif
