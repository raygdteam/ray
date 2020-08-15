#pragma once

#include <eastl/vector.h>

template <typename Type>
class Array : public eastl::vector<Type>
{
	void PushBack(const Type& value)
	{
		this->push_back(value);
	}
	
	void PushBack(Type&& value)
	{
		this->push_back(value);
	}

	bool IsEmpty() const
	{
		return this->empty();
	}
};
