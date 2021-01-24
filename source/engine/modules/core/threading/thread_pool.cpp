#include <core/threading/thread_pool.hpp>
#include <core/debug/assert.hpp>
#include <Windows.h>

struct ThreadPoolInternalData
{
	PTP_POOL Pool;
	_TP_CALLBACK_ENVIRON_V3 Environment;
	PTP_CLEANUP_GROUP CleanupGroup;

	Array<_TP_WORK*> Work;
};

void __stdcall _WorkCallback(PTP_CALLBACK_INSTANCE, PVOID context, PTP_WORK work)
{
	ThreadPoolJob* job = static_cast<ThreadPoolJob*>(context);
	job->Run();
}

ThreadPool::ThreadPool()
{
	_data = new ThreadPoolInternalData;

	_data->Pool = CreateThreadpool(nullptr);
	check(_data->Pool != nullptr);

	InitializeThreadpoolEnvironment(&_data->Environment);
	SetThreadpoolCallbackPool(&_data->Environment, _data->Pool);
	SetThreadpoolCallbackLibrary(&_data->Environment, GetModuleHandleA(nullptr));

	
	_data->CleanupGroup = CreateThreadpoolCleanupGroup();
	check(_data->CleanupGroup != nullptr);
	SetThreadpoolCallbackCleanupGroup(&_data->Environment, _data->CleanupGroup, nullptr);

	/*
	 * TODO: appropriate values;
	 * min = 2
	 * max = (cpus / 2) + 2 (???)
	 */
	SetThreadpoolThreadMinimum(_data->Pool, 2);
	SetThreadpoolThreadMaximum(_data->Pool, 4);
}

ThreadPool& ThreadPool::Begin()
{
	return gThreadPoolManager->Allocate();
}

void ThreadPool::SubmitWork(ThreadPoolJob** jobs, u64 count)
{
	for (u64 i = 0; i < count; ++i)
	{
		SubmitWork(jobs[i]);
	}
}

void ThreadPool::SubmitWork(Array<ThreadPoolJob*>& jobs)
{
	for (ThreadPoolJob* job : jobs)
	{
		SubmitWork(job);
	}
}

void ThreadPool::SubmitWork(ThreadPoolJob* job)
{
	PTP_WORK work = CreateThreadpoolWork(_WorkCallback, job, &_data->Environment);
	check(work != nullptr);
	
	_data->Work.PushBack(work);
	SubmitThreadpoolWork(work);
}

void ThreadPool::Wait()
{
	for (_TP_WORK* work : _data->Work)
	{
		WaitForThreadpoolWorkCallbacks(work, false);
		CloseThreadpoolWork(work);
	}
}

void ThreadPool::ResubmitWork()
{
	for (_TP_WORK* work : _data->Work)
	{
		SubmitThreadpoolWork(work);
	}
}

void ThreadPool::Reset()
{
	_data->Work.clear();
}

ThreadPoolManager::ThreadPoolManager()
{
}

ThreadPool& ThreadPoolManager::Allocate()
{
	_mutex.Enter();

	ThreadPool* ret = nullptr;

	if (_avaliableTps.empty())
	{
		ret = new ThreadPool;
		_avaliableTps.Push(ret);
	}
	else
	{
		ret = _avaliableTps.front();
		ret->Reset();
		_avaliableTps.Pop();
	}

	check(ret != nullptr);

	_mutex.Leave();
	return *ret;
}

void ThreadPoolManager::FreeContext(ThreadPool& pool)
{
	//check(pool != nullptr);

	pool.Reset();
	
	_mutex.Enter();
	_avaliableTps.Push(&pool);
	_mutex.Leave();
}

RAY_CORE_API ThreadPoolManager* gThreadPoolManager;
