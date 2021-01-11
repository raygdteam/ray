#pragma once
#include <core/core.hpp>
#include <core/lib/string.hpp>

class RAY_CORE_API Logger
{
	String _name;

public:
	Logger(pcstr name);
	~Logger();

	void Log(pcstr format, ...);
};
