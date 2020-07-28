#pragma once

template<typename T>
struct IScopedAllocation
{
	T data;

	IScopedAllocation(T d) : data(d)
	{ }

	~IScopedAllocation()
	{
		delete data;
	}
};
