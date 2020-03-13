#pragma once

#include <cstdint>
#include "core.hpp"

namespace ray
{
	class RAY_CORE_API Semaphore
	{
	protected:
		void* _data;
	public:
		Semaphore(uint32_t initial_count = 0);
		~Semaphore();
		bool try_get();
		void get();
		void put();
	};
}
