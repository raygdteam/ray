#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/lib/queue.hpp>
#include <core/threading/critical_section.hpp>

class ThreadPoolJob
{
public:
	ThreadPoolJob() = default;
	virtual ~ThreadPoolJob() = default;
	
	virtual void Run();
};

class RAY_CORE_API ThreadPool
{
	struct ThreadPoolInternalData* _data;
public:
	ThreadPool();

	static ThreadPool& Begin();

	void SubmitWork(ThreadPoolJob* jobs, u64 count);
	void SubmitWork(Array<ThreadPoolJob*>& jobs);
	void SubmitWork(ThreadPoolJob& job);

	template<typename Type = ThreadPoolJob&>
	void SubmitWork(Type jobs...)
	{
		SubmitWork(&jobs[0], sizeof...(jobs));
	}

	void Wait();

	void Reset();
};

class RAY_CORE_API ThreadPoolManager
{
	Array<ThreadPool*> _tpPool;
	Queue<ThreadPool*> _avaliableTps;
	CriticalSection _mutex;
public:
	ThreadPoolManager();
	
	ThreadPool* Allocate();
	void FreeContext(ThreadPool* pool);
};

extern RAY_CORE_API ThreadPoolManager* gThreadPoolManager;
