#pragma once

#include "common.hpp"

namespace ray::core::math::trigonometric
{
	template<typename type>
	inline type radians(type degrees)
	{
		/*check(std::numeric_limits<genType>::is_iec559, "'radians' only accept floating-point input");*/

		return degrees * static_cast<type>(0.01745329251994329576923690768489);
	}
}