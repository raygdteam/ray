#pragma once
#include <core/core.hpp>

namespace ray
{
template<typename Type>
class Array
{
	u64 _size = 0;
	u64 _capacity = 32;
	Type* _data = nullptr;
public:
	Array()
	{
		_size = 0;
		_capacity = 32;
		_data = new Type*[32];
	}

	/**
	 * Gets the current array size.
	 */
	u64 Size()
	{
		return _size;
	}

	/**
	 * Gets the allocated array capacity.
	 */
	u64 Capacity()
	{
		return _capacity;
	}

	void Push(Type data)
	{
		if (_size == _capacity)
		{
			// TODO: realloc
			return;
		}

		_data[_size] = data;
		_size += 1;
	}

	/**
	 * Iterator-like: returs the first element of array.
	 */
	Type* Begin()
	{
		return _data[0];
	}

	/**
	 * Iterator-like: returs the last element of array.
	 */
	Type* End()
	{
		return _data[_size - 1];
	}

	/* Aliases for c++ compilers since they require lowercase 'begin' / 'end'  */
	auto begin() -> decltype(this->Begin())	{ return Begin(); }
	auto end() -> decltype(this->End())	{ return End(); }

	
	bool IsEmpty()
	{
		return _size == 0;
	}
};
}
