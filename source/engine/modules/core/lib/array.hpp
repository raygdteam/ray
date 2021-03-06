#pragma once
#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <core/math/common.hpp>
#include <cstring>

#define RAY_USE_EA_STL

#ifdef RAY_USE_EA_STL

#include <eastl/vector.h>

template <typename Type>
class Array : public eastl::vector<Type>
{
public:
	void PushBack(const Type& value)
	{
		this->push_back(value);
	}
	
	void PushBack(Type&& value)
	{
		this->push_back(std::move(value));
	}

	template<typename... Args>
	void EmplaceBack(Args&&... args)
	{
		this->emplace_back(std::forward<Args>(args)...);
	}

	bool IsEmpty() const
	{
		return this->empty();
	}

	u64 Size() const noexcept
	{
		return this->size();
	}
	
	u64 Capacity() const noexcept
	{
		return this->capacity();
	}

	const Type* GetData() const
	{
		return this->data();
	}

	Type& At(size_t index)
	{
		return this->at(index);
	}

	void Clear() noexcept
	{
		this->clear();
	}

	void Resize(size_t newSize) noexcept
	{
		this->resize(newSize);
	}
};

#else

#include <core/memory/system_allocator.hpp>
#pragma clang diagnostic ignored "-Wreorder-ctor"

template<typename Type, typename Allocator = SystemAllocator>
class Array
{
private:
	Allocator _allocator;
	Type* _data;
	size_t _elementsCount;
	size_t _capacity;

public:
	Array() noexcept
		: _elementsCount(0)
		, _capacity(16)
		, _allocator()
		, _data(nullptr)
	{
		AllocateArray();
	}

	Array(const Array& lhs) noexcept = default;
	Array(Array&& rhs) noexcept = default;

	Array& operator = (const Array& lhs) noexcept = default;
	Array& operator = (Array&& rhs) noexcept = default;

	explicit Array(size_t elementsCount) noexcept
		: _elementsCount(elementsCount)
		, _allocator()
		, _data(nullptr)
	{
		_capacity = 1;

		while (elementsCount >= _capacity)
			_capacity <<= 1;

		AllocateArray();
	}

	explicit Array(size_t elementsCount, size_t capacity) noexcept
		: _allocator()
		, _data(nullptr)
	{
		ray_assert(elementsCount < capacity, "Capacity must be greater then elements count!")
		check((capacity & (capacity - 1)) == 0)

		_elementsCount = elementsCount;
		_capacity = capacity;

		AllocateArray();
	}

	~Array()
	{
		_capacity = 0;
		_elementsCount = 0;
		delete[] _data;
	}

public:
	void PushBack(const Type& element) noexcept
	{
		if (_elementsCount > _capacity)
		{
			AllocateArray();
		}

		_data[_elementsCount++] = element;
	}

	void EmplaceBack(Type&& element) noexcept
	{
		if (_elementsCount > _capacity)
		{
			AllocateArray();
		}

		_data[_elementsCount++] = std::move(element);
	}

	template<typename... Args>
	void EmplaceBack(Args&&... args) noexcept
	{
		if (_elementsCount > _capacity)
		{
			AllocateArray();
		}

		_data[_elementsCount++] = Type(std::forward<Args>(args)...);
	}

	void Insert(const Type& element, const size_t index) noexcept
	{
		if (_elementsCount + 1 > _capacity)
		{
			AllocateArray();
		}

		memcpy(&_data[index + 1], &_data[index], _elementsCount - index);
		_data[index] = element;
	}

	void Insert(Type&& element, const size_t index) noexcept
	{
		if (_elementsCount + 1 > _capacity)
		{
			AllocateArray();
		}

		memcpy(&_data[index + 1], &_data[index], _elementsCount - index);
		_data[index] = std::move(element);
	}

	void Remove(const size_t index) noexcept
	{
		memcpy(&_data[index], &_data[index + 1], _elementsCount - (index + 1));
		--_elementsCount;
	}

	void Clear() noexcept
	{
		_elementsCount = 0;
	}

	Type& At(const size_t index) const noexcept
	{
		ray_assert(index < _elementsCount, "Out of bounds!")
		return _data[index];
	}

	Type* begin() const noexcept
	{
		return _data;
	}

	Type* end() const noexcept
	{
		return _data + _elementsCount;
	}

public:
	bool IsEmpty() const noexcept
	{
		return _elementsCount == 0;
	}

	Type* GetData() const noexcept
	{
		return _data;
	}

	size_t Size() const noexcept
	{
		return _elementsCount;
	}

	size_t Capacity() const noexcept
	{
		return _capacity;
	}

	void SetCapacity(const size_t newCapacity) noexcept
	{
		if (newCapacity < _capacity)
		{
			return;
		}

		_capacity = newCapacity;
		AllocateArray();
	}

	void Resize(const size_t newSize) noexcept
	{
		if (newSize < _elementsCount)
		{
			return;
		}

		_elementsCount = newSize;

		while (_elementsCount >= _capacity)
			_capacity <<= 1;

		AllocateArray();
	}

private:
	void AllocateArray()
	{
		Type* newData = new Type[_capacity];
	
		if (_data != nullptr)
		{
			memcpy(newData, _data, _elementsCount);
			delete[] _data;
		}

		_data = newData;
	}

private:
	void* operator new(size_t size) noexcept
	{
		return _allocator.Allocate(size);
	}

	void operator delete(void* mem) noexcept
	{
		_allocator.Free(mem);
	}

	void* operator new[](size_t size) noexcept
	{
		return _allocator.Allocate(size);
	}

	void operator delete[](void* mem) noexcept
	{
		_allocator.Free(mem);
	}

};


#endif // RAY_USE_EA_STL
