#pragma once

#include "core/core.hpp"
#include "core/lib/format.hpp"

/* TODO: get rid of it */
#include <windows.h>
#include <debugapi.h>

struct LogLevel
{
	const char* Name;
	const char* Color;
};

LogLevel LogLevelWarning { .Name = "warning", .Color = "\033[33m" };
LogLevel LogLevelError { .Name = "error", .Color = "\033[31m" };
LogLevel LogLevelDebug { .Name = "debug", .Color = "\033[34m" };

class Logger
{
	const char* name;

public:
	Logger(const char* name) : name(name) {}

	void Log(const char* format)
	{
#ifndef RAY_RELEASE
		const char* buffer = Format("[{}] {}\n", this->name, format);

		printf("%s", buffer);
		OutputDebugStringA(buffer);

		delete[] buffer;
#endif
	}

	template<typename type>
	void Log(const char* format, type argument)
	{
		this->Log(Format(format, argument));
	}

	template<typename type, typename ... Arguments>
	void Log(const char* format, type argument, Arguments ... arguments)
	{
		this->Log(Format(format, argument, arguments...));
	}

	void Log(LogLevel level, const char* format)
	{
		this->Log(Format("[{}{}\033[0m] {}", level.Color, level.Name, format));
	}

	template<typename type>
	void Log(LogLevel level, const char* format, type argument)
	{
		this->Log(Format("[{}{}\033[0m] {}", level.Color, level.Name, Format(format, argument)));
	}

	template<typename type, typename ... Arguments>
	void Log(LogLevel level, const char* format, type argument, Arguments ... arguments)
	{
		this->Log(Format("[{}{}\033[0m] {}", level.Color, level.Name, Format(format, argument, arguments...)));
	}
};

/*
#define LWarning(logger_, format_, ...) logger_.Log(ray::core::logger::warning, ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));
#define LError(logger_, format_, ...) logger_.Log(ray::core::logger::error, ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));

#ifdef RAY_DEBUG
#define LDebug(logger_, format_, ...) logger_.Log(ray::core::logger::debug, ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));
#else
#define LDebug(logger_, format_, ...)
#endif

#define LLog(logger_, format_, ...) logger_.Log(ray::core::format::format("[{}:{}:{}] [{}] {}", __builtin_FILE(), __builtin_LINE(), __builtin_COLUMN(), __builtin_FUNCTION(), ray::core::format::format(format_, __VA_ARGS__)));
*/
