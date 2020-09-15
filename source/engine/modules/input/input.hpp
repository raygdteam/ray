#pragma once

#include <core/core.hpp>

#ifdef RAY_BUILD_INPUT
#define RAY_INPUT_API RAY_DLLEXPORTS
#else
#define RAY_INPUT_API RAY_DLLIMPORT
#endif

namespace Ray
{
	void RAY_INPUT_API Initialize();

	class RAY_INPUT_API Input
	{
	public:
		Input();

		void Update(u16& x, u16& y);

	private:
		struct
		{
			struct
			{
				u16 x = 0;
				u16 y = 0;
			} coordinates;
		} mouse;
	};
}
