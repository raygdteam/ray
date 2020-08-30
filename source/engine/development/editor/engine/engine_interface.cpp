#include <editor/engine/engine_interface.hpp>
#include "engine.hpp"

IEngine* EditorInterface::CreateEditorEngine()
{
	return new EditorEngine;
}

