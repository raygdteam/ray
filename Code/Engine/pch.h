#pragma once

//Core
#include "../Core/core.h"

//engine
#include "platform.hpp"

namespace CPU = ray::core::hardware;

//Windows Libs
#ifdef RAY_PLATFORM_WIN


#include <Windows.h>


#endif //RAY_PLATFORM_WIN

//spdlog
#include <spdlog/spdlog.hpp>

//std libs
#include <vector>
#include <string>
#include <algorithm>