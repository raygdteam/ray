#pragma once
#include <core/core.hpp>

struct CoreGlobals
{
	struct CpuInfo
	{
		u32 NumCores;
		u32 NumThreads;
	} CpuInfo;
	
	
};

struct EngineGlobals
{
	bool IsInEditor;
};

extern RAY_CORE_API CoreGlobals gCoreGlobals;
extern RAY_CORE_API EngineGlobals gEngineGlobals;
