#pragma once
#include "color_buffer.hpp"
#include <renderer_core/resources/depth_buffer.hpp>

extern const u32 SWAP_CHAIN_BUFFER_COUNT;
extern u32 gCurrentBuffer;
extern ColorBuffer gDisplayPlane[];
extern ColorBuffer gSceneColorBuffer; // is not being used now
extern DepthBuffer gDepthBuffer;
