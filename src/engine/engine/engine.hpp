#pragma once
#include "../Core/application.h"
#include "world/Level.hpp"

namespace ray::engine {

	class engine
	{
	public:
		static void start(core::application* app);
		static void preinitialize(core::application* app);
		static void initialize();
		static Level get_active_level();
		static void schedule_renderer_reload();
	private:
		static void run();
		static void destroy();
	};

}
