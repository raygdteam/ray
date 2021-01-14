#pragma once
#include <core/core.hpp>

//temporary
#include <Windows.h>

	template<typename MemoryPool>
	class MemoryManager
	{
	public:
		~MemoryManager()
		{
			Destroy();
		}

		void Initialize(size_t preferredSize) noexcept;
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

	template<typename MemoryPool>
	void MemoryManager<MemoryPool>::Initialize(size_t preferredSize) noexcept
	{
		_preferredPoolSize = preferredSize;

		_listOfPools.Buffer = static_cast<MemoryPool*>(VirtualAlloc(nullptr, KB(64), MEM_COMMIT, PAGE_READWRITE));
		new (&_listOfPools.Buffer[0]) MemoryPool;
		_listOfPools.Buffer[0].Create(preferredSize, 0);
		_listOfPools.ElementsCount += 1;
	}

	template<typename MemoryPool>
	void MemoryManager<MemoryPool>::Destroy() noexcept
	{
		if (_listOfPools.ElementsCount != -1u)
		{
			for (size_t i = 0; i < _listOfPools.ElementsCount; ++i)
				_listOfPools.Buffer[i].Destroy();

			VirtualFree(_listOfPools.Buffer, 0, MEM_RELEASE);
			_listOfPools.ElementsCount = -1u;
		}
	}

	template<typename MemoryPool>
	MemoryPool& MemoryManager<MemoryPool>::RequestPool(size_t requestedSize) noexcept
	{
		auto& pool = _listOfPools.Buffer[_listOfPools.ElementsCount - 1];

		if (pool.IsEnough(requestedSize))
			return pool;

		size_t size = requestedSize > _preferredPoolSize ? requestedSize : _preferredPoolSize;
		return CreatePool(size);
	}

	template<typename MemoryPool>
	MemoryPool& MemoryManager<MemoryPool>::CreatePool(size_t requestedSize) noexcept
	{
		new (&_listOfPools.Buffer[_listOfPools.ElementsCount]) MemoryPool;
		_listOfPools.Buffer[_listOfPools.ElementsCount].Create(requestedSize, _listOfPools.ElementsCount);
		return _listOfPools.Buffer[_listOfPools.ElementsCount++];
	}