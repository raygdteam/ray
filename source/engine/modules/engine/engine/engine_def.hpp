#pragma once
#include <core/core.hpp>

#if RAY_BUILD_ENGINE
#define RAY_ENGINE_API RAY_DLLEXPORTS
#else
#define RAY_ENGINE_API RAY_DLLIMPORT
#endif

