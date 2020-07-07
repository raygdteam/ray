#pragma once
#include <core/core.hpp>

class IThread
{
	void* _rawHandle = nullptr;
	static u64 RawThreadEntry(void* thisptr);
public:
	IThread();
	virtual void ThreadEntry() = 0;
	void Start();
};
