#pragma once
#include <core/core.hpp>

struct Archive
{
	virtual void Read(void* buffer, u64 size) = 0;
	virtual void Write(void* buffer, u64 size) = 0;

    template<typename T>
    void Read(T& value)
    {
        Read(&value, sizeof(value));
    }

    template<typename T>
    void Write(const T& value)
    {
        Write((void*)&value, sizeof(value));
    }
};
