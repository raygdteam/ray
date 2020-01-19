#pragma once

#include <winnt.h>

namespace ray::config::model
{
	struct
	{
		u16 x;
		u16	y;
		u16 width = 1024;
		u16 height = 768;
	} window;
}