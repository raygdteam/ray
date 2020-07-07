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
