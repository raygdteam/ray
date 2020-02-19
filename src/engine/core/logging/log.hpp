#pragma once
#include "core/core.hpp"
#include "spdlog/fmt/fmt.h"

namespace ray::logging
{

struct RAY_CORE_API ILog
{
	ILog(string name) : _name(name)
	{}

	template <typename... Args>
	void log(const char* format, const Args& ... args)
	{
		_log(fmt::format(format, args...));
	}
private:
	void _log(string text);

	string _name;
};


}
