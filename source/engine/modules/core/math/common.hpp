#pragma once
#include <core/core.hpp>

namespace ray::core::math
{
	template<typename Type>
	inline Type AlignUpWithMask(Type value, size_t mask)
	{
		return (Type)(((size_t)(value + mask) & ~mask));
	}

	template<typename Type>
	inline Type AlignUp(Type value, size_t alignment)
	{
		return AlignUpWithMask(value, alignment - 1);
	}

	template <typename T>
	inline bool IsAligned(T value, size_t alignment)
	{
		return 0 == ((size_t)value & (alignment - 1));
	}

	template <typename T> 
	inline T DivideByMultiple(T value, size_t alignment)
	{
		return (T)((value + alignment - 1) / alignment);
	}

	f32 Sin(f32 val);
	f32 Cos(f32 val);
	f32 Tan(f32 val);

	f32 Asin(f32 val);
	f32 Acos(f32 val);
}