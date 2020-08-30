#include <core/module/module.hpp>

#include <editor/engine/engine_interface.hpp>

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
		static EditorInterface _interface;
		return &_interface;
	}
};

REGISTER_MODULE(EditorModule);
