#include "pch.hpp"
#include "lock.hpp"
#include <ray/os/include.hpp>

namespace ray::threading
{

Lock::Lock()
{
	_handle = new CRITICAL_SECTION;
	InitializeCriticalSection((LPCRITICAL_SECTION)_handle);
}

Lock::~Lock()
{
	DeleteCriticalSection((LPCRITICAL_SECTION)_handle);
	delete _handle;
}

void Lock::Enter()
{
	EnterCriticalSection((LPCRITICAL_SECTION)_handle);
}

void Lock::TryEnter()
{
	TryEnterCriticalSection((LPCRITICAL_SECTION)_handle);
}

void Lock::Leave()
{
	LeaveCriticalSection((LPCRITICAL_SECTION)_handle);
}
}
