#pragma once

#include <memory>
#include <core/core.hpp>

template <typename Type>//, typename Allocator>
class BasicArray
{
public:

};

template <typename Type>//, typename Allocator>
class Vector : public BasicArray<Type>//, Allocator>
{
public:
	Vector() : _begin(nullptr), _end(nullptr), _capacity(0) {}

	Vector(size_t size) : _begin(new Type[size]), _end(this->_begin), _capacity(this->_begin + size) {}

	void PushBack(const Type& value)
	{
		if (this->_end < this->_capacity)
			new((void*)this->_end++) Type(value);
		/*else
			;*/
	}

	void PushBack(Type&& value)
	{

		if (this->_end < this->_capacity)
			new((void*)this->_end++) Type(std::move(value));
		/*else
			;*/
	}

	template<class... Args>
	void EmplaceBack(Args&&... args)
	{
		if (this->_end < this->_capacity)
			new((void*)this->_end++) Type(std::forward<Args>(args)...);
		/*else
			;*/
	}

	bool IsEmpty() const
	{
		return (this->_begin == this->_end);
	}

	u64 Size() const noexcept
	{
		return (u64)(this->_end - this->_begin);
	}

	u64 Capacity() const noexcept
	{
		return (u64)(this->_capacity - this->_begin);
	}

	const Type* GetData() const
	{
		return this->_begin;
	}

	Type& At(size_t index)
	{
		return *(this->_begin + index);
	}

private:
	Type* _begin;
	Type* _end;

	Type* _capacity;
};