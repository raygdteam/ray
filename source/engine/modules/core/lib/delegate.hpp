#pragma once
#include <core/lib/array.hpp>
#include <eastl/functional.h>

template<typename R>
using Function = eastl::function<R>;

template<typename R, typename... Args>
class MulticastDelegate;

template<typename R, typename... Args>
class MulticastDelegate<R(Args...)>
{
public:
	using Fn = Function<R(Args...)>;
private:
	Array<Fn> _handlers;
public:
	void Register(Fn fn)
	{
		_handlers.PushBack(fn);
	}

	void Invoke(Args... args)
	{
		for (Fn handler : _handlers)
		{
			handler.operator()(args...);
		}
	}
};

void Test1(int, bool)
{
	
}
void Test2(int, bool)
{

}

void test()
{
	MulticastDelegate<void(int, bool)> handler;
	handler.Register(Test1);
	handler.Register(Test2);
	handler.Invoke(1, true);
}
