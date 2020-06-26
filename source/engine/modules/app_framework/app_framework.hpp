#pragma once
#include <core/core.hpp>

#ifdef RAY_BUILD_APPFRAMEWORK
#define RAY_APPFRAMEWORK_API RAY_DLLEXPORTS
#else
#define RAY_APPFRAMEWORK_API RAY_DLLIMPORT
#endif
