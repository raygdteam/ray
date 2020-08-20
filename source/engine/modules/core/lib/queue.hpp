#pragma once
#include <core/core.hpp>
#include <eastl/queue.h>

template<typename T>
class Queue : public eastl::queue<T>
{
	using Super = eastl::queue<T>;
	
public:
	Queue(std::initializer_list<T> ilist) : Super(ilist)
	{
		
	}

	bool IsEmpty() const
	{
		return this->empty();
	}

	void Push(const T& value)
	{
		this->push(value);
	}
	
	void Push(T&& x)
	{
		this->push(x);
	}

	void Pop()
	{
		this->pop();
	}

	u64 Size()
	{
		return this->size();
	}
};
