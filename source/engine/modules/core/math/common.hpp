#pragma once

namespace ray::core::math
{
	template<typename Type>
	inline Type AlignUpWithMask(Type value, size_t mask)
	{
		return (Type)(((size_t)(value + mask) & ~mask));
	}
}