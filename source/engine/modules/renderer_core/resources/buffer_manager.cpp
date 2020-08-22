#include "buffer_manager.hpp"

namespace ray::renderer_core_api::globals
{
	const u32 SWAP_CHAIN_BUFFER_COUNT = 3;

	resources::ColorBuffer gDisplayPlane[SWAP_CHAIN_BUFFER_COUNT] = 
	{
		resources::ColorBuffer(1.f, 0.f, 0.f, 1.f),
		resources::ColorBuffer(1.f, 0.f, 0.f, 1.f),
		resources::ColorBuffer(1.f, 0.f, 0.f, 1.f)
	};
	resources::ColorBuffer gSceneColorBuffer = resources::ColorBuffer(1.f, 1.f, 1.f, 1.f);
}