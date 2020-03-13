#pragma once
#include "core.hpp"
#include "TaskSystem.h"

#define RAY_SCOPED_LOCK(lock) ray::ScopedLock LOCK__ ## __LINE__(lock);

namespace ray
{
	class Lock
	{
	protected:
		uint32_t _locked;
	public:
		constexpr Lock(bool locked = false) : _locked(locked ? 1 : 0)
		{
		}

		inline bool try_lock() { return !_locked && !cas(&_locked, 0, 1); }

		inline void lock()
		{
			TaskSystem::yield_until([](void* data)
			{
				return ((Lock*)data)->try_lock();
			}, this);
		}

		inline void unlock() { _locked = 0; }
	};

	class ScopedLock
	{
	protected:
		Lock& _lock;
	public:
		inline ScopedLock(Lock& lock) : _lock(lock) { _lock.lock(); }
		inline ~ScopedLock() { _lock.unlock(); }
	};
}
