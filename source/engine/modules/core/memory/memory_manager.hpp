#pragma once
#include <core/core.hpp>

//temporary
#include <Windows.h>

namespace ray
{
	template<typename MemoryPool>
	class MemoryManager
	{
	public:
		~MemoryManager()
		{
			Destroy();
		}

		void Initialize() noexcept;
		void Destroy() noexcept;

		MemoryPool& RequestPool(size_t requestedSize) noexcept;

		void SetPreferredPoolSize(size_t size) noexcept
		{
			_preferredPoolSize = size;
		}

	private:
		MemoryPool& CreatePool(size_t requestedSize) noexcept;

	private:
		size_t _preferredPoolSize;

		struct
		{
			MemoryPool* Buffer;
			size_t ElementsCount = 0;
			size_t MaxElementsCount = KB(64) / sizeof(MemoryPool);
		} _listOfPools;

	};
}