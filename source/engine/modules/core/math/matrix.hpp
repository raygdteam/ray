#pragma once
#include <core/math/vector.hpp>
#include <core/debug/assert.hpp>

#include <initializer_list>
#include <intrin.h>

// 'F' stands for 'fast'
template<u8 M_Row, u8 N_Column>
struct FMatrix;

template<>
struct FMatrix<2, 2>
{
	FVector<2> Row[2];

	FMatrix(std::initializer_list<FVector<2>> list)
	{
		check(list.size() <= 2);
		
		for (u8 i = 0; i < list.size(); ++i)
		{
			Row[i] = list.begin()[i];
		}
	}

	FMatrix<2, 2> Add(const FMatrix<2, 2>& param)
	{
		return FMatrix<2, 2> {
			FVector<2> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y },
			FVector<2> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y },
		};
	}

	FMatrix<2, 2> Substract(const FMatrix<2, 2>& param)
	{
		return FMatrix<2, 2> {
			FVector<2> {.x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y },
			FVector<2> {.x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y },
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

	FMatrix(std::initializer_list<FVector<3>> list)
	{
		check(list.size() <= 3);
		
		for (u8 i = 0; i < list.size(); ++i)
		{
			Row[i] = list.begin()[i];
		}
	}
	
	FMatrix<3, 3> Add(const FMatrix<3, 3>& param)
	{
		return FMatrix<3, 3> {
			FVector<3> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y, .z = Row[0].z + param.Row[0].z },
			FVector<3> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y, .z = Row[1].z + param.Row[1].z },
			FVector<3> {.x = Row[2].x + param.Row[2].x, .y = Row[2].y + param.Row[2].y, .z = Row[2].z + param.Row[2].z },
		};
	}

	FMatrix<3, 3> Substract(const FMatrix<3, 3>& param)
	{
		return FMatrix<3, 3> {
			FVector<3> {.x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y, .z = Row[0].z - param.Row[0].z },
			FVector<3> {.x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y, .z = Row[1].z - param.Row[1].z },
			FVector<3> {.x = Row[2].x - param.Row[2].x, .y = Row[2].y - param.Row[2].y, .z = Row[2].z - param.Row[2].z },
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

	FMatrix(std::initializer_list<FVector<4>> list)
	{
		check(list.size() <= 4);
		
		for (u8 i = 0; i < list.size(); ++i)
		{
			Row[i] = list.begin()[i];
		}
	}

	FMatrix<4, 4> Add(const FMatrix<4, 4>& param)
	{
		return FMatrix<4, 4> {
			FVector<4> {.x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y, .z = Row[0].z + param.Row[0].z, .w = Row[0].w + param.Row[0].w },
			FVector<4> {.x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y, .z = Row[1].z + param.Row[1].z, .w = Row[1].w + param.Row[1].w },
			FVector<4> {.x = Row[2].x + param.Row[2].x, .y = Row[2].y + param.Row[2].y, .z = Row[2].z + param.Row[2].z, .w = Row[2].w + param.Row[2].w },
			FVector<4> {.x = Row[3].x + param.Row[3].x, .y = Row[3].y + param.Row[3].y, .z = Row[3].z + param.Row[3].z, .w = Row[3].w + param.Row[3].w },
		};
	}

	FMatrix<4, 4> Substract(const FMatrix<4, 4>& param)
	{
		return FMatrix<4, 4> {
			FVector<4> {.x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y, .z = Row[0].z - param.Row[0].z, .w = Row[0].w - param.Row[0].w },
			FVector<4> {.x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y, .z = Row[1].z - param.Row[1].z, .w = Row[1].w - param.Row[1].w },
			FVector<4> {.x = Row[2].x - param.Row[2].x, .y = Row[2].y - param.Row[2].y, .z = Row[2].z - param.Row[2].z, .w = Row[2].w - param.Row[2].w },
			FVector<4> {.x = Row[3].x - param.Row[3].x, .y = Row[3].y - param.Row[3].y, .z = Row[3].z - param.Row[3].z, .w = Row[3].w - param.Row[3].w },
		};
	}

	FVector4 Transform(const FVector4& vec)
	{
		/* Любезно подсмотрено у UE4 */
		const __m128* matrix = reinterpret_cast<const __m128*>(Row);
		__m128 vector = _mm_load_ps(reinterpret_cast<const float*>(&vec));
		__m128 tempX, tempY, tempZ, tempW;

		#define MASK(a) ((a) | ((a)<<2) | ((a)<<4) | ((a)<<6))
		
		tempX = _mm_shuffle_ps(vector, vector, MASK(0));
		tempY = _mm_shuffle_ps(vector, vector, MASK(1));
		tempZ = _mm_shuffle_ps(vector, vector, MASK(2));
		tempW = _mm_shuffle_ps(vector, vector, MASK(3));

		#undef MASK

		tempX = _mm_mul_ps(tempX, matrix[0]);
		tempY = _mm_mul_ps(tempY, matrix[0]);
		tempZ = _mm_mul_ps(tempZ, matrix[0]);
		tempW = _mm_mul_ps(tempW, matrix[0]);

		tempX = _mm_add_ps(tempX, tempY);
		tempZ = _mm_add_ps(tempZ, tempW);
		tempX = _mm_add_ps(tempX, tempZ);

		FVector4 result;
		_mm_store_ps(reinterpret_cast<float*>(&result), tempX);

		return result;
	}

	static FMatrix<4, 4> Scale(f32 scalar) 
	{
		return FMatrix<4, 4> {
			{ .x = scalar,	.y = 0.f,		.z = 0.f,		.w = 0.f },
			{ .x = 0.f,		.y = scalar,	.z = 0.f,		.w = 0.f },
			{ .x = 0.f,		.y = 0.f,		.z = scalar,	.w = 0.f },
			{ .x = 0.f,		.y = 0.f,		.z = 0.f,		.w = 1.f },
		};
	}

	static FMatrix<4, 4> Scale(FVector<3> scale) 
	{
		return FMatrix<4, 4> {
			{ .x = scale.x, .y = 0.f, 		.z = 0.f, 		.w = 0.f },
			{ .x = 0.f,		.y = scale.y,	.z = 0.f,		.w = 0.f },
			{ .x = 0.f,		.y = 0.f,		.z = scale.z,	.w = 0.f },
			{ .x = 0.f,		.y = 0.f,		.z = 0.f,		.w = 1.f },
		};
	}

	FVector4 operator[](u8 idx)
	{
		check(idx <= 4);
		return Row[idx];
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
