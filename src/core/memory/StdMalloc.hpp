#pragma once
#include "IMalloc.hpp"

namespace ray::core::memory
{

	class StdMalloc 
		: public IMalloc
	{
	public:
		~StdMalloc() override {}

		void* Alloc(size_t Size, size_t Alignment = 16) override;
		void Free(void*);
		void* Realloc(void*, size_t, size_t) override;
		inline size_t GetAllocationSize(void*) override;

	};

}



