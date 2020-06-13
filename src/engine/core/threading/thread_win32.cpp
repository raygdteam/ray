#include <pch.hpp>
#include "core.hpp"

// win32 ray::threading::ithread implementation
#ifdef RAY_PLATFORM_WIN

#include "thread.hpp"
#include <Windows.h>
#include <process.h>

namespace ray::threading
{

unsigned long _thread_entry(void* pThis)
{
	// TODO dark: There should be an additional thread initialization here.
	static_cast<IThread*>(pThis)->Run();
	
	return 0;
}

void IThread::Run()
{
	(void)0;
}

void IThread::Start()
{
	//_beginthread(&_thread_entry, 4 * 1024, this);
	_handle = CreateThread(0, 0, &_thread_entry, this, 0, 0);
}

IThread::~IThread()
{
	WaitForSingleObject(_handle, INFINITE);
	CloseHandle(_handle);
}
}

#endif