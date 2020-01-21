#include <pch.hpp>
#include "core.h"

// win32 ray::threading::thread implementation
#ifdef RAY_PLATFORM_WIN

#include "thread.hpp"
#include <Windows.h>
#include <process.h>

namespace ray::threading
{

unsigned long thread::_thread_entrypoint(void* pthis)
{
	auto function = *static_cast<std::function<void()>*>(pthis);
	function();
	return 0;
}

thread::thread(std::function<void()> delegate) : _delegate(delegate), _handle(nullptr)
{
	_handle = CreateThread(0, 0, &_thread_entrypoint, &_delegate, 0, 0);
}

void thread::join()
{
	WaitForSingleObject(_handle, INFINITY);
}
}

#endif