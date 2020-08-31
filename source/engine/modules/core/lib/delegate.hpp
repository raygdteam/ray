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
			if (handler != nullptr)
			{
				handler.operator()(args...);
			}
		}
	}
};
