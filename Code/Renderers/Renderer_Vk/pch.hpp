#pragma once

#include <ray/os/preinclude.hpp>
#include <ray/type/extensions.hpp>

#include "Engine/irenderer.hpp"
#include "Core/core.h"
#include "Engine/platform.hpp"

#include <spdlog/spdlog.hpp>
#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/ext/matrix_clip_space.hpp>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
