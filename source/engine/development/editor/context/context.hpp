#pragma once
#include <core/core.hpp>

struct EditorContext
{
	struct EditorUiContext
	{
		u32 PrimaryDockspaceId;
		u32 LevelDockspaceId;
	} UiContext;
};

extern EditorContext gEditorContext;
