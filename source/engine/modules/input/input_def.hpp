#pragma once
#include <core/core.hpp>

#ifdef RAY_BUILD_INPUT
#define RAY_INPUT_API RAY_DLLEXPORTS
#else
#define RAY_INPUT_API RAY_DLLIMPORT
#endif
