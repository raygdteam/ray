#include "pch.hpp"

#include <ray/os/preinclude.hpp>
#include <spdlog/spdlog.hpp>
#include "core.h"

// Подключаем заголовочные файлы.
#include "file_system.hpp"

#include "object.hpp"
class testobj : ray::object
{
	GENERATED_BODY;
};

namespace ray::core {

	void preinitialize()
	{
		static bool _preinitialized = false;
		RAY_ASSERT(!_preinitialized, "Core was already preinitialized")
		_preinitialized = true;

		spdlog::info("Preinitializing Ray core version {}.{}.{} (codename: {})", RAY_VERSION_MAJOR, RAY_VERSION_MINOR,
			RAY_VERSION_PATCH, RAY_VERSION_CODENAME);

		file_system::initialize();
	}

}