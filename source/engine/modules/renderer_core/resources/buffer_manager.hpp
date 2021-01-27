#pragma once
#include "color_buffer.hpp"
#include <renderer_core/resources/depth_buffer.hpp>
#include <renderer_core/renderer_core.hpp>

extern const u32 SWAP_CHAIN_BUFFER_COUNT;
extern u32 gCurrentBuffer;
extern ColorBuffer gDisplayPlane[];
extern RAY_RENDERERCORE_API ColorBuffer gSceneColorBuffer;
extern RAY_RENDERERCORE_API ColorBuffer gEditorColorBuffer;
extern DepthBuffer gDepthBuffer;
