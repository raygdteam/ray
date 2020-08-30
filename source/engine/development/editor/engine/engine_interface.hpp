#pragma once
#include <core/module/module.hpp>
#include <editor/engine/engine.hpp>

class EDITOR_API EditorInterface : public IModuleInterface
{
public:
	IEngine* CreateEditorEngine();
};
