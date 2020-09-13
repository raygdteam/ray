#pragma once
#include <core/core.hpp>

class RAY_CORE_API ConditionVariable
{
	void* _handle = nullptr;
public:
	ConditionVariable();
	
	/** Signal all waiting threads */
	void Signal() const;

	/** Wait for this CV to become true */
	void Wait() const;

	/** Aliases */
	void Wake() const
	{ Signal(); }
	
	void WaitFor() const
	{ Wait(); }
};
