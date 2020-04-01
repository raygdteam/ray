#include "pch.hpp"
#include "Mutex.h"
#include "core/core.hpp"

#ifdef RAY_PLATFORM_LINUX

#include <pthread.h>

namespace ray
{

#define _handle static_cast<pthread_mutex_t*>(_data)

Mutex::Mutex()
{
	pthread_mutex_init(_handle, 0);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(_handle);
}

void Mutex::lock()
{
	pthread_mutex_lock(_handle);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(_handle);
}

void Mutex::try_lock()
{
	pthread_mutex_trylock(_handle);
}

}

#endif
