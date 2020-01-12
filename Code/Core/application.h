#pragma once
#include "types.h"

namespace ray::core {

	struct __declspec(novtable) application
	{
		virtual void on_startup() = 0;
		virtual pcstr get_resources_path() = 0;
	};

}
