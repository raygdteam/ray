#include "condition_variable.hpp"
#include <core/os/miniwin.hpp>

ConditionVariable::ConditionVariable()
{
	_handle = WinApi::CreateEvent(false, false, nullptr);
}

void ConditionVariable::Signal() const
{
	WinApi::SetEvent(_handle);
}

void ConditionVariable::Wait() const
{
	WinApi::WaitForEvent(_handle);
}
