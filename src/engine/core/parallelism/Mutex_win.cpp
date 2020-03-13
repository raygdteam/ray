#include "pch.hpp"
#include "Mutex.h"

#include <windows.h>
#include "memory/memory.hpp"

using namespace ray;

#define _handle ((CRITICAL_SECTION*)_data)

Mutex::Mutex()
{
	_data = new CRITICAL_SECTION;
	InitializeCriticalSection(_handle);
}

Mutex::~Mutex()
{
	DeleteCriticalSection(_handle);
	delete _data;
}

bool Mutex::try_lock()
{
	return TryEnterCriticalSection(_handle) != FALSE;
}

void Mutex::lock()
{
	EnterCriticalSection(_handle);
}

void Mutex::unlock()
{
	LeaveCriticalSection(_handle);
}
