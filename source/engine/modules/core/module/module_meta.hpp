#pragma once
#include <core/core.hpp>

struct ModuleMeta
{
	char Name[64];
	bool GameModule;
};

struct IModuleInterface
{ };

struct IModule
{
	ModuleMeta Meta;

	virtual void OnLoad() = 0;
	virtual void OnUnload() = 0;

	virtual IModuleInterface* QueryModuleInterface() = 0;

	virtual ~IModule() = default;
};

