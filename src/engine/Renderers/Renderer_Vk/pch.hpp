#pragma once

#include <ray/os/preinclude.hpp>
#include <ray/type/extensions.hpp>

#include "engine/engine/irenderer.hpp"
#include "engine/core/core.hpp"

#include <spdlog/spdlog.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <GLM/glm.hpp>
#include <GLM/ext/matrix_transform.hpp>
#include <GLM/ext/matrix_clip_space.hpp>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
