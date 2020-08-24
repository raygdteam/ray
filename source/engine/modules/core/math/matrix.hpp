#pragma once
#include <core/math/vector.hpp>

// 'F' stands for 'fast'
template<u8 M_Row, u8 N_Column>
struct FMatrix;

template<>
struct FMatrix<2, 2>
{
	FVector<2> Row[2];

	FMatrix<2, 2> Add(const FMatrix<2, 2>& param)
	{
		return FMatrix<2, 2> {
			.Row = {
				FVector<2> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y },
				FVector<2> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y },
			}
		};
	}

	FMatrix<2, 2> Substract(const FMatrix<2, 2>& param)
	{
		return FMatrix<2, 2> {
			.Row = {
				FVector<2> {.x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y },
				FVector<2> {.x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y },
			}
		};
	}

	FMatrix<2, 2> operator+=(const FMatrix<2, 2>& param)
	{
		return Add(param);
	}

	FMatrix<2, 2> operator-=(const FMatrix<2, 2>& param)
	{
		return Substract(param);
	}
};

template<>
struct FMatrix<3, 3>
{
	FVector<3> Row[3];
	
	FMatrix<3, 3> Add(const FMatrix<3, 3>& param)
	{
		return FMatrix<3, 3> {
			.Row = {
				FVector<3> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y, .z = Row[0].z + param.Row[0].z },
				FVector<3> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y, .z = Row[1].z + param.Row[1].z },
				FVector<3> {.x = Row[2].x + param.Row[2].x, .y = Row[2].y + param.Row[2].y, .z = Row[2].z + param.Row[2].z },
			}
		};
	}

	FMatrix<3, 3> Substract(const FMatrix<3, 3>& param)
	{
		return FMatrix<3, 3> {
			.Row = {
				FVector<3> {.x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y, .z = Row[0].z - param.Row[0].z },
				FVector<3> {.x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y, .z = Row[1].z - param.Row[1].z },
				FVector<3> {.x = Row[2].x - param.Row[2].x, .y = Row[2].y - param.Row[2].y, .z = Row[2].z - param.Row[2].z },
			}
		};
	}

	FMatrix<3, 3> operator+=(const FMatrix<3, 3>& param)
	{
		return Add(param);
	}

	FMatrix<3, 3> operator-=(const FMatrix<3, 3>& param)
	{
		return Substract(param);
	}
};

template<>
struct FMatrix<4, 4>
{
	FVector<4> Row[4];

	FMatrix<4, 4> Add(const FMatrix<4, 4>& param)
	{
		return FMatrix<4, 4> {
			.Row = {
				FVector<4> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y, .z = Row[0].z + param.Row[0].z, .w = Row[0].w + param.Row[0].w },
				FVector<4> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y, .z = Row[1].z + param.Row[1].z, .w = Row[1].w + param.Row[1].w },
				FVector<4> {.x = Row[2].x + param.Row[2].x, .y = Row[2].y + param.Row[2].y, .z = Row[2].z + param.Row[2].z, .w = Row[2].w + param.Row[2].w },
				FVector<4> {.x = Row[3].x + param.Row[3].x, .y = Row[3].y + param.Row[3].y, .z = Row[3].z + param.Row[3].z, .w = Row[3].w + param.Row[3].w },
			}
		};
	}

	FMatrix<4, 4> Substract(const FMatrix<4, 4>& param)
	{
		return FMatrix<4, 4> {
			.Row = {
				FVector<4> {.x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y, .z = Row[0].z - param.Row[0].z, .w = Row[0].w - param.Row[0].w },
				FVector<4> {.x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y, .z = Row[1].z - param.Row[1].z, .w = Row[1].w - param.Row[1].w },
				FVector<4> {.x = Row[2].x - param.Row[2].x, .y = Row[2].y - param.Row[2].y, .z = Row[2].z - param.Row[2].z, .w = Row[2].w - param.Row[2].w },
				FVector<4> {.x = Row[3].x - param.Row[3].x, .y = Row[3].y - param.Row[3].y, .z = Row[3].z - param.Row[3].z, .w = Row[3].w - param.Row[3].w },
			}
		};
	}

	FMatrix<4, 4> operator+=(const FMatrix<4, 4>& param)
	{
		return Add(param);
	}

	FMatrix<4, 4> operator-=(const FMatrix<4, 4>& param)
	{
		return Substract(param);
	}
};

using FMatrix2 = FMatrix<2, 2>; using FMatrix2x2 = FMatrix<2, 2>;
using FMatrix3 = FMatrix<3, 3>; using FMatrix3x3 = FMatrix<3, 3>;
using FMatrix4 = FMatrix<4, 4>; using FMatrix4x4 = FMatrix<4, 4>;
