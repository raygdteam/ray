#pragma once
#include <core/core.hpp>
#include <core/lib/string.hpp>

enum EditorCmdType
{
	eLoadLevel
};

struct EditorCommand
{
	EditorCmdType Type;
};

struct EditorCommand_LoadLevel : public EditorCommand
{
	String Path;
	
	EditorCommand_LoadLevel()
	{
		this->Type = eLoadLevel;
	}
};
