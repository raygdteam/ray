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

		spdlog::set_pattern("[%D %T] [%^%l%$]: %v");

		spdlog::info("Preinitializing Ray core version {}.{}.{} (codename: {})", RAY_VERSION_MAJOR, RAY_VERSION_MINOR,
			RAY_VERSION_PATCH, RAY_VERSION_CODENAME);

		file_system::initialize();

		// TODO: костыль. перенести в platform или чтото
		char* path = new char[360];
		size_t size = 360;
		_dupenv_s(&path, &size, "localappdata");
		std::string s(path);
		s.append("\\ray");
		file_system::query_mount(s.c_str(), "/local");
		delete path;
	}

}