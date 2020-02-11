#include "pch.hpp"

// Hacky way to override new's on Release.
#ifdef RAY_BUILD_RELEASE
#undef RAY_BUILD_RELEASE
#include "memory.hpp"

MEMORY_OVERRIDE;

#endif