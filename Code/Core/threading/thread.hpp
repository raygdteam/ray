#pragma once
#include <pch.hpp>
#include <functional>

namespace ray::threading
{

class thread
{
	void* _handle;
	std::function<void()> _delegate;

	static unsigned long _thread_entrypoint(void* pthis);
public:
	thread(std::function<void()>);
	void join();
};
}
