#pragma once

#include "../../Engine/platform.hpp"
#include "../../Engine/imodel_component.hpp"
#include "../../Core/core.h"

#include <spdlog/spdlog.hpp>

#define FREE_MEM(ptr) { if(ptr) { ptr->Release(); ptr = 0x0; } }
