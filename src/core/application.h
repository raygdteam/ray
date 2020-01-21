#pragma once
#include "types.h"

namespace ray::core 
{

enum application_type
{
	unknown,
	game,
	editor,
	tool
};

struct application_info
{
	application_type type;

	// both

};

struct __declspec(novtable) application
{
	virtual void on_startup() = 0;
	virtual pcstr get_resources_path() = 0;
};

}
