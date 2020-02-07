#pragma once
#include "core/core.hpp"
#include "Iterator.hpp"

namespace raytl
{
	
template<typename Type>
class Array
{
public:
	Array();
	Array(size_t);
	Array(const Array&);
	~Array();
	
	void Add(Type NewElement);
	size_t Size() const { return size; }
	Type* Data() { return data; }

	Type& operator[](size_t Index);

	Iterator<Type> begin()
	{
		Iterator<Type> it(data);
		return it;
	}

	Iterator<Type> end()
	{
		Iterator<Type> it(data + size);
		return it;
	}

private:
	Type* data;
	size_t size;
	size_t capacity;

};

	
}



