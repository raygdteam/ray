#pragma once
#include <core/core.hpp>

// temporary
#include <Windows.h>

namespace ray
{
	struct MemorySegment
	{
		size_t Offset;
		size_t Size;
	};

	class IMemoryPool
	{
	public:
		virtual ~IMemoryPool() { Destroy(); }

		virtual void Create(size_t maxMemoryPoolSize) noexcept
		{
			_listOfSegments.Buffer = static_cast<MemorySegment*>(VirtualAlloc(nullptr, KB(64), MEM_COMMIT, PAGE_READWRITE));
			_maxMemoryPoolSize = maxMemoryPoolSize;
			_offset = 0;
			_availableSize = maxMemoryPoolSize;
		}

		virtual void Destroy() noexcept
		{
			VirtualFree(_listOfSegments.Buffer, 0, MEM_RELEASE);
		}

		bool IsEnough(size_t size) const noexcept
		{
			return _offset + size <= _maxMemoryPoolSize;
		}

	protected:
		size_t _maxMemoryPoolSize;
		void* _pool;
		size_t _offset;
		size_t _availableSize;

		struct
		{
			MemorySegment* Buffer;
			size_t ElementsCount = 0;
			size_t MaxElementsCount = KB(64) / sizeof(MemorySegment);

			size_t FreedSpace = 0;
		} _listOfSegments;

	};
}