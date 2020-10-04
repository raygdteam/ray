#pragma once

#include <core/lib/array.hpp>
#include <core/threading/critical_section.hpp>
#include "gpu_buffer.hpp"

#define MIN_PLACED_BUFFER_SIZE 64 * 1024

namespace ray::renderer_core_api
{
	enum BuddyAllocationStrategy
	{
		ePlacedResourceStrategy,
		eManualSuballocationStrategy
	};

	struct BuddyBlock
	{

	};

	class BuddyAllocator
	{

	};
}