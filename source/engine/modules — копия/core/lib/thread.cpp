#include "thread.hpp"

extern "C"
{
#include <kernel/kernel_thread.h>
}

u64 IThread::RawThreadEntry(void* thisptr)
{
	((IThread*)thisptr)->ThreadEntry();
	return 0;
}

IThread::IThread()
{
	_rawHandle = krnlCreateThread(0, &RawThreadEntry, this, 0);
}

void IThread::Start()
{
	krnlResumeThread(_rawHandle);
}

void IThread::Join()
{
	krnlJoinThread(_rawHandle);
}

void IThread::Name(const char* const name)
{
	krnlSetThreadName(name);
}

struct ILambdaThread : IThread
{
	ILambdaThread(std::function<void()> fn)
	{
		_fn = fn;
	}

	void ThreadEntry()
	{
		_fn();
	}
private:
	std::function<void()> _fn;
};

IThread* IThread::Start(const std::function<void()>& fn)
{
	ILambdaThread* thread = new ILambdaThread(fn);
	return thread;
}
