#pragma once
#include <core/core.hpp>

namespace ray
{

class RAY_CORE_API CriticalSection
{
	void* _data;
public:
	CriticalSection();
	~CriticalSection();

	void Enter();
	void TryEnter();

	void Leave();
};

}
