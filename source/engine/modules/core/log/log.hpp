#pragma once

#include "core/core.hpp"
#include "core/lib/format.hpp"

#include <windows.h>
#include <debugapi.h>

namespace ray::core::log
{
	struct level
	{
		const char* name;
		const char* color;
	};

	level warning { .name = "warning", .color = "\033[33m" };
	level error { .name = "error", .color = "\033[31m" };
	level debug { .name = "debug", .color = "\033[34m" };

	class logger
	{
		const char* name;

	public:
		logger(const char* name) : name(name) {}

		void Log(const char* format)
		{
			const char* unnamed = format::format("[{}] {}", this->name, format);

			printf("%s", unnamed);
			OutputDebugStringA(unnamed);
		}

		template<typename type>
		void Log(const char* format, type argument)
		{
			this->Log(format::format(format, argument));
		}

		template<typename type, typename ... Arguments>
		void Log(const char* format, type argument, Arguments ... arguments)
		{
			this->Log(format::format(format, argument, arguments...));
		}

		void Log(level level, const char* format)
		{
			this->Log(format::format("[{}{}\033[0m] {}", level.color, level.name, format));
		}

		template<typename type>
		void Log(level level, const char* format, type argument)
		{
			this->Log(format::format("[{}{}\033[0m] {}", level.color, level.name, format::format(format, argument)));
		}

		template<typename type, typename ... Arguments>
		void Log(level level, const char* format, type argument, Arguments ... arguments)
		{
			this->Log(format::format("[{}{}\033[0m] {}", level.color, level.name, format::format(format, argument, arguments...)));
		}
	};
}

#define LWarning(logger_, format_, ...) logger_.Log(ray::core::logger::warning, ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));
#define LError(logger_, format_, ...) logger_.Log(ray::core::logger::error, ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));

#ifdef RAY_DEBUG
#define LDebug(logger_, format_, ...) logger_.Log(ray::core::logger::debug, ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));
#else
#define LDebug(logger_, format_, ...)
#endif

#define LLog(logger_, format_, ...) logger_.Log(ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));