#include "pch.hpp"
#include "array.hpp"
#include "platform/platform_memory.hpp"

#define DEFAULT_CAPACITY_SIZE 16

using namespace ray::core::platform;

namespace raytl
{
	template <typename Type>
	Array<Type>::Array()
		: size(0)
		, capacity(DEFAULT_CAPACITY_SIZE)
		, data(nullptr)
	{
		data = new Array<Type>[capacity];
	}

	template <typename Type>
	Array<Type>::Array(const Array<Type>& OtherArray)
	{
		size = OtherArray.size;
		capacity = OtherArray.capacity;
		data = new Array<Type>[capacity];
		if(!data)
		{
			PlatformMemory::Memcpy(OtherArray.data, data, size);
		}
	}

	template <typename Type>
	Array<Type>::Array(size_t NewSize)
	{
		size = NewSize;
		capacity = DEFAULT_CAPACITY_SIZE;
		if(size < capacity)
		{
			data = new Array<Type>[capacity];
			return;
		}

		while (capacity <= size)
			capacity <<= 1;

		data = new Array<Type>[capacity];
		
	}

	template <typename Type>
	Array<Type>::~Array()
	{
		if (!data)
			delete[] data;
	}

	template <typename Type>
	void Array<Type>::Add(Type NewElement)
	{
		if(size <= capacity)
		{
			data[size++] = NewElement;
			return;
		}

		capacity <<= 1;
		Type* NewData = new Array[capacity];
		PlatformMemory::Memcpy(data, NewData, size);
		delete[] data;
		data = NewData;
		data[size++] = NewElement;
		
	}

	template <typename Type>
	Type& Array<Type>::operator[](size_t Index)
	{
		RAY_ASSERT(Index < size, "Out of range!")
		return data[Index];
	}



}