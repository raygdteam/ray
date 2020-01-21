#pragma once
#include "IMalloc.hpp"

namespace ray::core::memory
{

	class StdMalloc 
		: public IMalloc
	{
	public:
		~StdMalloc() override {}

		void* Alloc(size_t Size, size_t Alignment = 4) override;
		void Free(void*);
		void* Realloc(void*, size_t) override;

	};

}



