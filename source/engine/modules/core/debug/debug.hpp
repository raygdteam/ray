#pragma once
#include <core/core.hpp>

namespace ray
{

class RAY_CORE_API Debug
{
	static void handleSignal(int sc);
public:
	bool Initialize();

	
};
 
}
