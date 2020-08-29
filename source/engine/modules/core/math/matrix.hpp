#pragma once
#include <core/math/vector.hpp>
#include <core/debug/assert.hpp>

#include <initializer_list>
#include <intrin.h>

#include "common.hpp"

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
			{ .x = Row[0].x + param.Row[0].x, .y = Row[0].y + param.Row[0].y, .z = Row[0].z + param.Row[0].z, .w = Row[0].w + param.Row[0].w },
			{ .x = Row[1].x + param.Row[1].x, .y = Row[1].y + param.Row[1].y, .z = Row[1].z + param.Row[1].z, .w = Row[1].w + param.Row[1].w },
			{ .x = Row[2].x + param.Row[2].x, .y = Row[2].y + param.Row[2].y, .z = Row[2].z + param.Row[2].z, .w = Row[2].w + param.Row[2].w },
			{ .x = Row[3].x + param.Row[3].x, .y = Row[3].y + param.Row[3].y, .z = Row[3].z + param.Row[3].z, .w = Row[3].w + param.Row[3].w },
		};
	}

	FMatrix<4, 4> Substract(const FMatrix<4, 4>& param)
	{
		return FMatrix<4, 4> {
			{ .x = Row[0].x - param.Row[0].x, .y = Row[0].y - param.Row[0].y, .z = Row[0].z - param.Row[0].z, .w = Row[0].w - param.Row[0].w },
			{ .x = Row[1].x - param.Row[1].x, .y = Row[1].y - param.Row[1].y, .z = Row[1].z - param.Row[1].z, .w = Row[1].w - param.Row[1].w },
			{ .x = Row[2].x - param.Row[2].x, .y = Row[2].y - param.Row[2].y, .z = Row[2].z - param.Row[2].z, .w = Row[2].w - param.Row[2].w },
			{ .x = Row[3].x - param.Row[3].x, .y = Row[3].y - param.Row[3].y, .z = Row[3].z - param.Row[3].z, .w = Row[3].w - param.Row[3].w },
		};
	}

	FMatrix<4, 4> Multiply(const FMatrix& param)
	{
		// TODO: nice-ify
		FVector<4> A[4] = { Row[0], Row[1], Row[2], Row[3] };
		FVector<4> B[4] = { param.Row[0], param.Row[1], param.Row[2], param.Row[3] };

		return FMatrix<4, 4> {
			{
				(A[0].x * B[0].x) + (A[0].y * B[1].x) + (A[0].z * B[2].x) + (A[0].w * B[3].x),
				(A[1].x * B[0].x) + (A[1].y * B[1].x) + (A[1].z * B[2].x) + (A[1].w * B[3].x),
				(A[2].x * B[0].x) + (A[2].y * B[1].x) + (A[2].z * B[2].x) + (A[2].w * B[3].x),
				(A[3].x * B[0].x) + (A[3].y * B[1].x) + (A[3].z * B[2].x) + (A[3].w * B[3].x)
			},
			{
				(A[0].x * B[0].y) + (A[0].y * B[1].y) + (A[0].z * B[2].y) + (A[0].w * B[3].y),
				(A[1].x * B[0].y) + (A[1].y * B[1].y) + (A[1].z * B[2].y) + (A[1].w * B[3].y),
				(A[2].x * B[0].y) + (A[2].y * B[1].y) + (A[2].z * B[2].y) + (A[2].w * B[3].y),
				(A[3].x * B[0].y) + (A[3].y * B[1].y) + (A[3].z * B[2].y) + (A[3].w * B[3].y)
			},
			{
				(A[0].x * B[0].z) + (A[0].y * B[1].z) + (A[0].z * B[2].z) + (A[0].w * B[3].z),
				(A[1].x * B[0].z) + (A[1].y * B[1].z) + (A[1].z * B[2].z) + (A[1].w * B[3].z),
				(A[2].x * B[0].z) + (A[2].y * B[1].z) + (A[2].z * B[2].z) + (A[2].w * B[3].z),
				(A[3].x * B[0].z) + (A[3].y * B[1].z) + (A[3].z * B[2].z) + (A[3].w * B[3].z)
			},
			{
				(A[0].x * B[0].w) + (A[0].y * B[1].w) + (A[0].z * B[2].w) + (A[0].w * B[3].w),
				(A[1].x * B[0].w) + (A[1].y * B[1].w) + (A[1].z * B[2].w) + (A[1].w * B[3].w),
				(A[2].x * B[0].w) + (A[2].y * B[1].w) + (A[2].z * B[2].w) + (A[2].w * B[3].w),
				(A[3].x * B[0].w) + (A[3].y * B[1].w) + (A[3].z * B[2].w) + (A[3].w * B[3].w)
			},
		};
	}

	FMatrix<4, 4>* Translate(FVector<3> const& vector)
	{
		Row[3] = Row[0].Multiply(vector.x) + Row[1].Multiply(vector.y) + Row[2].Multiply(vector.z) + Row[3];
		return this;
	}

	FMatrix<4, 4> Inverse(FMatrix<4, 4> matrix)
	{
		f32 coef00 = matrix.Row[2].z * matrix.Row[3].w - matrix.Row[3].z * matrix.Row[2].w;
		f32 coef02 = matrix.Row[1].z * matrix.Row[3].w - matrix.Row[3].z * matrix.Row[1].w;
		f32 coef03 = matrix.Row[1].z * matrix.Row[2].w - matrix.Row[2].z * matrix.Row[1].w;

		f32 coef04 = matrix.Row[2].y * matrix.Row[3].w - matrix.Row[3].y * matrix.Row[2].w;
		f32 coef06 = matrix.Row[1].y * matrix.Row[3].w - matrix.Row[3].y * matrix.Row[1].w;
		f32 coef07 = matrix.Row[1].y * matrix.Row[2].w - matrix.Row[2].y * matrix.Row[1].w;

		f32 coef08 = matrix.Row[2].y * matrix.Row[3].z - matrix.Row[3].y * matrix.Row[2].z;
		f32 coef10 = matrix.Row[1].y * matrix.Row[3].z - matrix.Row[3].y * matrix.Row[1].z;
		f32 coef11 = matrix.Row[1].y * matrix.Row[2].z - matrix.Row[2].y * matrix.Row[1].z;

		f32 coef12 = matrix.Row[2].x * matrix.Row[3].w - matrix.Row[3].x * matrix.Row[2].w;
		f32 coef14 = matrix.Row[1].x * matrix.Row[3].w - matrix.Row[3].x * matrix.Row[1].w;
		f32 coef15 = matrix.Row[1].x * matrix.Row[2].w - matrix.Row[2].x * matrix.Row[1].w;

		f32 coef16 = matrix.Row[2].x * matrix.Row[3].z - matrix.Row[3].x * matrix.Row[2].z;
		f32 coef18 = matrix.Row[1].x * matrix.Row[3].z - matrix.Row[3].x * matrix.Row[1].z;
		f32 coef19 = matrix.Row[1].x * matrix.Row[2].z - matrix.Row[2].x * matrix.Row[1].z;

		f32 coef20 = matrix.Row[2].x * matrix.Row[3].y - matrix.Row[3].x * matrix.Row[2].y;
		f32 coef22 = matrix.Row[1].x * matrix.Row[3].y - matrix.Row[3].x * matrix.Row[1].y;
		f32 coef23 = matrix.Row[1].x * matrix.Row[2].y - matrix.Row[2].x * matrix.Row[1].y;

		FVector<4> fac0 { .x = coef00, .y = coef00, .z = coef02, .w = coef03 };
		FVector<4> fac1 { .x = coef04, .y = coef04, .z = coef06, .w = coef07 };
		FVector<4> fac2 { .x = coef08, .y = coef08, .z = coef10, .w = coef11 };
		FVector<4> fac3 { .x = coef12, .y = coef12, .z = coef14, .w = coef15 };
		FVector<4> fac4 { .x = coef16, .y = coef16, .z = coef18, .w = coef19 };
		FVector<4> fac5 { .x = coef20, .y = coef20, .z = coef22, .w = coef23 };

		FVector<4> vec0 { .x = matrix.Row[1].x, .y = matrix.Row[0].x, .z = matrix.Row[0].x, .w = matrix.Row[0].x };
		FVector<4> vec1 { .x = matrix.Row[1].y, .y = matrix.Row[0].y, .z = matrix.Row[0].y, .w = matrix.Row[0].y };
		FVector<4> vec2 { .x = matrix.Row[1].z, .y = matrix.Row[0].z, .z = matrix.Row[0].z, .w = matrix.Row[0].z };
		FVector<4> vec3 { .x = matrix.Row[1].w, .y = matrix.Row[0].w, .z = matrix.Row[0].w, .w = matrix.Row[0].w };

		FVector<4> inv0 { vec1 * fac0 - vec2 * fac1 + vec3 * fac2 };
		FVector<4> inv1 { vec0 * fac0 - vec2 * fac3 + vec3 * fac4 };
		FVector<4> inv2 { vec0 * fac1 - vec1 * fac3 + vec3 * fac5 };
		FVector<4> inv3 { vec0 * fac2 - vec1 * fac4 + vec2 * fac5 };

		FVector<4> signA { .x = 1.f, .y = -1.f, .z = 1.f, .w = -1.f };
		FVector<4> signB { .x = -1.f, .y = 1.f, .z = -1.f, .w = 1.f };
		FMatrix<4, 4> inverse { inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB };

		FVector<4> row0 { .x = inverse.Row[0].x, .y = inverse.Row[1].x, .z = inverse.Row[2].x, .w = inverse.Row[3].x };

		FVector<4> dot0(matrix.Row[0] * row0);
		f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

		f32 oneOverDeterminant = 1.f / dot1;

		return inverse.Scale(oneOverDeterminant);
	}

	FMatrix<4, 4> Rotate(f32 angle, const FVector3& vector)
	{
		f32 const c = ray::core::math::Cos(angle);
		f32 const s = ray::core::math::Sin(angle);

		FVector<3> axis(Normalize(vector));
		FVector<3> temp(axis.Multiply((1.f - c)));

		FMatrix<4, 4> rotate
		{
			FVector<4>{.x = c + temp.x * axis.x, .y = temp.x * axis.y + s * axis.z, .z = temp.x * axis.z - s * axis.y },
			FVector<4>{.x = temp.y * axis.x - s * axis.z, .y = c + temp.y * axis.y , .z = temp.y * axis.z + s * axis.x },
			FVector<4>{.x = temp.z * axis.x + s * axis.y, .y = temp.z * axis.y - s * axis.x, .z = c + temp.z * axis.z }
		};

		return rotate;

		/*mat<4, 4, T, Q> Result;
		Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
		Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
		Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
		Result[3] = m[3];
		return Result;*/
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

inline FMatrix4x4 Orthographic(f32 left, f32 right, f32 bottom, f32 top)
{
	return FMatrix<4, 4> 
	{
		FVector<4>
		{
			.x = 2.f / (right - left),
			.y = 0.f,
			.z = 0.f,
			.w = 0.f,
		},
		FVector<4>
		{
			.x = 0.f,
			.y = 2.f / (top - bottom),
			.z = 0.f,
			.w = 0.f,
		},
		FVector<4>
		{
			.x = 0.f,
			.y = 0.f,
			.z = -1.f,
			.w = 0.f,
		},
		FVector<4>
		{
			.x = -(right + left) / (right - left),
			.y = -(top + bottom) / (top - bottom),
			.z = 0.f,
			.w = 0.f,
		},
	};
}