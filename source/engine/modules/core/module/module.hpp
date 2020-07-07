#pragma once
#include <core/core.hpp>
#include <core/module/module_meta.hpp>
#include <core/lib/result.hpp>

enum ModuleLoadError : u8
{
	eSuccess = 0,
	eOsError,
	eNotARayModule,
};

class RAY_CORE_API ModuleManager
{
public:
	Result<IModule*, ModuleLoadError> LoadModule(pcstr name);
};
