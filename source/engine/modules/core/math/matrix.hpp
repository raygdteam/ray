#pragma once
#include <core/math/vector.hpp>

// 'F' stands for 'fast'
template<u8 M_Row, u8 N_Column>
struct FMatrix;

template<>
struct FMatrix<2, 2>
{
	FVector<2> Row[2];

	FMatrix<2, 2> Add(FMatrix<2, 2> param)
	{
		return FMatrix<2, 2> {
			.Row = {
				FVector<2> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y },
				FVector<2> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y },
			}
		};
	}
};

template<>
struct FMatrix<3, 3>
{
	FVector<3> Row[3];
	
	FMatrix<3, 3> Add(FMatrix<3, 3> param)
	{
		return FMatrix<3, 3> {
			.Row = {
				FVector<3> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y, .z = Row[0].z + param.Row[0].z },
				FVector<3> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y, .z = Row[0].z + param.Row[0].z },
				FVector<3> {.x = Row[2].x + param.Row[2].x, .y = Row[2].y + param.Row[2].y, .z = Row[2].z + param.Row[2].z },
			}
		};
	}
};

template<>
struct FMatrix<4, 4>
{
	FVector<4> Row[4];
};
