#include <core/core.hpp>
#include "common.hpp"
#include <math.h>

namespace ray::core::math
{
	f32 Sin(f32 val)
	{
		return sinf(val);
	}

	f32 Cos(f32 val)
	{
		return cosf(val);
	}

	f32 Tan(f32 val)
	{
		return tanf(val);
	}

	f32 Asin(f32 val)
	{
		return asinf(val);
	}

	f32 Acos(f32 val)
	{
		return acosf(val);
	}

	f32 InverseSqrt(f32 val)
	{
		return 1.f / sqrt(val);
	}
}
