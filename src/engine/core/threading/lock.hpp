#pragma once

namespace ray::threading
{


class Lock
{
	void* _handle;
public:
	Lock();
	~Lock();

	void Enter();
	void TryEnter();
	void Leave();
};
}
