#pragma once
#include <core/core.hpp>

class RAY_CORE_API CriticalSection
{
	void* _data;
public:
	CriticalSection();
	~CriticalSection();

	/**
	 * Locks the mutex.
	 */
	void Enter() const;

	/**
	 * Attempts to enter mutex.
	 * Returns false on failure.
	 */
	[[nodiscard]] bool TryEnter() const;

	/**
	 * Unlocks the mutex.
	 */
	void Leave() const;
};
