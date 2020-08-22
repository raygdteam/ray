#pragma once
#include <core/core.hpp>
#include <core/debug/debug.hpp>


#ifdef RAY_RELEASE
#define assert(condition, msg)
#else
#define STRINGIFY(argument) #argument
#define ray_assert(condition, msg) do { \
	if (!(condition)) { \
		Debug::__Internal_HandleAssertionFailure(#condition, __FILE__, __LINE__, msg); \
	} \
	} while(false);

#define check(condition) do { \
	if (!(condition)) { \
		Debug::__Internal_HandleAssertionFailure(#condition, __FILE__, __LINE__, "<NO-MESSAGE>"); \
	} \
	} while(false);

#endif