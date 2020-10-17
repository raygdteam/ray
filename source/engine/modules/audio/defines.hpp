#pragma once

#include <core/core.hpp>

#ifdef RAY_BUILD_AUDIO
#define RAY_AUDIO_API RAY_DLLEXPORTS
#else
#define RAY_AUDIO_API RAY_DLLIMPORT
#endif
