#pragma once
#include "critical_section.hpp"

extern "C"
{
#include <kernel/kernel_win32_critical_section.h>
}


CriticalSection::CriticalSection() : _data(nullptr)
{
	_data = krnlWin32CreateCriticalSection();
}

CriticalSection::~CriticalSection()
{
	krnlWin32DeleteCriticalSection(_data);
}

void CriticalSection::Enter() const
{
	krnlWin32EnterCriticalSection(_data);
}

bool CriticalSection::TryEnter() const
{
	return bool(krnlWin32TryEnterCriticalSection(_data));
}

void CriticalSection::Leave() const
{
	krnlWin32LeaveCriticalSection(_data);
}
