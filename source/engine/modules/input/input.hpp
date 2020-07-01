#pragma once

#include "listener.hpp"

namespace ray::input
{
	void update();

	void add_listener(ray::input::listener*);
	void add_remove(ray::input::listener*);
}