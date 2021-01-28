#include "buffer_manager.hpp"
#include <core/lib/string.hpp>

const u32 SWAP_CHAIN_BUFFER_COUNT = 3;
u32 gCurrentBuffer = 0;

ColorBuffer gDisplayPlane[SWAP_CHAIN_BUFFER_COUNT] =
{
	ColorBuffer(1.f, 0.f, 0.f, 1.f),
	ColorBuffer(0.f, 1.f, 0.f, 1.f),
	ColorBuffer(0.f, 0.f, 1.f, 1.f)
};
ColorBuffer gSceneColorBuffer = ColorBuffer(1.f, 1.f, 1.f, 1.f);
ColorBuffer gEditorColorBuffer = ColorBuffer(1.f, 1.f, 1.f, 1.f);
DepthBuffer gDepthBuffer = DepthBuffer(); 
