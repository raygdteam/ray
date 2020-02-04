#pragma once
#include <pch.hpp>
#include <functional>

namespace ray::threading
{

class IThread
{
	void* _handle;
public:
	void Run();
	void Start();

	~IThread();
};

}
