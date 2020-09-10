#pragma once
#include <core/core.hpp>

class RAY_CORE_API ConditionVariable
{
	void* _handle = nullptr;
public:
	ConditionVariable();
	
	/** Signal all waiting threads */
	void Signal();

	/** Wait for this CV to become true */
	void Wait();

	/** Aliases */
	void Wake()
	{ Signal(); }
	
	void WaitFor()
	{ Wait(); }
};
