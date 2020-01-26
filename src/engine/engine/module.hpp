#pragma once
#include "pch.h"
#include "core/memory/memory.hpp"

namespace ray::modules
{

class IModuleInterface
{
	
};

template<class T>
struct IRegisterGameModule
{
	// TODO:
	IRegisterGameModule(pcstr)
	{
		
	}
};


}

#if defined(RAY_BUILD_RELEASE)
#define RAY_MODULE(name, moduleClass) \
	static ray::modules::IRegisterNameModule<moduleClass>(name) ##name##; \
	MEMORY_OVERRIDE;
#else
// TODO: 
#define RAY_MODULE(name, moduleClass) 
#endif