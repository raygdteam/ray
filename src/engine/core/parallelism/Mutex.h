#pragma once

#include "core/core.hpp"

namespace ray
{
	class RAY_CORE_API Mutex
	{
	protected:
		void* _data;
	public:
		Mutex();
		~Mutex();
		bool try_lock();
		void lock();
		void unlock();
	};

	class RAY_CORE_API ScopedMutex
	{
	protected:
		Mutex& _mutex;
	public:
		inline ScopedMutex(Mutex& mutex) : _mutex(mutex) { _mutex.lock(); }
		inline ~ScopedMutex() { _mutex.unlock(); }
	};
}
