#pragma once
#include "core.hpp"

namespace ray::core::hardware
{

	enum Feature : u32
	{
		SSE = 1 << 0,
		SSE2 = 1 << 1,
		SSE3 = 1 << 2,
		SSSE3 = 1 << 3,
		SSE4_1 = 1 << 4,
		SSE4_2 = 1 << 5,
		SSE4A = 1 << 6,
		SSE5 = 1 << 7,
		AVX = 1 << 8
	};

	struct CPU
	{
		struct CacheInfo
		{
			u16 cache_line_size;
			u16 cache_L1_data_size;
			u16 cache_L1_instructions_size;
			u16 cache_L2_size;
			u16 cache_L3_size;

			u16 threads_count;
			u16 frequency;
		} cache;

		u32 features;
	};

	enum Vendor
	{
		AMD,
		INTEL,
		NONE
	};

	struct processor
	{
		CPU	cpu;
		Vendor vendor;
		pcstr model;
	};

	void	init_processor_windows(processor*);
	void	init_processor_linux(processor*);
	bool	has_feature(processor*, Feature);
	void	clear_processor_info(processor*);

	//optimization
	__forceinline void prefetch_block(const void* InPtr, s32 NumBytes = 1);
	__forceinline void prefetch(void const* x, s32 offset = 0);

}

