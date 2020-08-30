#include <core/module/module.hpp>

class EditorModule : public IModule
{
public:
	void OnLoad() override
	{
		this->Meta = ModuleMeta{ .Name = "editor", .GameModule = false };
	}
	
	void OnUnload() override {}
	
	IModuleInterface* QueryModuleInterface() override
	{
		return nullptr;
	}
};

REGISTER_MODULE(EditorModule);
