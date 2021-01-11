#pragma once

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
};
