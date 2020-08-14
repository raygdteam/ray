#pragma once
#include <core/core.hpp>
#include <functional>

class RAY_CORE_API IThread
{
	void* _rawHandle = nullptr;
	static u64 RawThreadEntry(void* thisptr);
public:
	IThread();
	virtual void ThreadEntry() = 0;
	void Start();

	void Join();
	
	static IThread* Start(const std::function<void()>& fn);
};
