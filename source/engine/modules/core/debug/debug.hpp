#pragma once
#include <core/core.hpp>

class RAY_CORE_API Debug
{
	static void handleSignal(int sc);
public:
	Debug();
	static void __Internal_HandleAssertionFailure(pcstr expression, pcstr file, u16 line, pcstr message);
	
};
