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
			Row[i] = list.begin()[i];
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

	FMatrix<4, 4>* Translate(FVector<3> const& vector)
	{
		this->operator[](3) = this->operator[](0).Multiply(vector.x) + this->operator[](1).Multiply(vector.y) + this->operator[](2).Multiply(vector.z) + this->operator[](3);
		return this;
	}

	FMatrix<4, 4> Inverse(FMatrix<4, 4> matrix)
	{
		f32 Coef00 = matrix[2].z * matrix[3].w - matrix[3].z * matrix[2].w;
		f32 Coef02 = matrix[1].z * matrix[3].w - matrix[3].z * matrix[1].w;
		f32 Coef03 = matrix[1].z * matrix[2].w - matrix[2].z * matrix[1].w;

		f32 Coef04 = matrix[2].y * matrix[3].w - matrix[3].y * matrix[2].w;
		f32 Coef06 = matrix[1].y * matrix[3].w - matrix[3].y * matrix[1].w;
		f32 Coef07 = matrix[1].y * matrix[2].w - matrix[2].y * matrix[1].w;

		f32 Coef08 = matrix[2].y * matrix[3].z - matrix[3].y * matrix[2].z;
		f32 Coef10 = matrix[1].y * matrix[3].z - matrix[3].y * matrix[1].z;
		f32 Coef11 = matrix[1].y * matrix[2].z - matrix[2].y * matrix[1].z;

		f32 Coef12 = matrix[2].x * matrix[3].w - matrix[3].x * matrix[2].w;
		f32 Coef14 = matrix[1].x * matrix[3].w - matrix[3].x * matrix[1].w;
		f32 Coef15 = matrix[1].x * matrix[2].w - matrix[2].x * matrix[1].w;

		f32 Coef16 = matrix[2].x * matrix[3].z - matrix[3].x * matrix[2].z;
		f32 Coef18 = matrix[1].x * matrix[3].z - matrix[3].x * matrix[1].z;
		f32 Coef19 = matrix[1].x * matrix[2].z - matrix[2].x * matrix[1].z;

		f32 Coef20 = matrix[2].x * matrix[3].y - matrix[3].x * matrix[2].y;
		f32 Coef22 = matrix[1].x * matrix[3].y - matrix[3].x * matrix[1].y;
		f32 Coef23 = matrix[1].x * matrix[2].y - matrix[2].x * matrix[1].y;

		FVector<4> Fac0{ .x = Coef00, .y = Coef00, .z = Coef02, .w = Coef03 };
		FVector<4> Fac1{ .x = Coef04, .y = Coef04, .z = Coef06, .w = Coef07 };
		FVector<4> Fac2{ .x = Coef08, .y = Coef08, .z = Coef10, .w = Coef11 };
		FVector<4> Fac3{ .x = Coef12, .y = Coef12, .z = Coef14, .w = Coef15 };
		FVector<4> Fac4{ .x = Coef16, .y = Coef16, .z = Coef18, .w = Coef19 };
		FVector<4> Fac5{ .x = Coef20, .y = Coef20, .z = Coef22, .w = Coef23 };

		FVector<4> Vec0{ .x = matrix[1].x, .y = matrix[0].x, .z = matrix[0].x, .w = matrix[0].x };
		FVector<4> Vec1{ .x = matrix[1].y, .y = matrix[0].y, .z = matrix[0].y, .w = matrix[0].y };
		FVector<4> Vec2{ .x = matrix[1].z, .y = matrix[0].z, .z = matrix[0].z, .w = matrix[0].z };
		FVector<4> Vec3{ .x = matrix[1].w, .y = matrix[0].w, .z = matrix[0].w, .w = matrix[0].w };

		FVector<4> Inv0{ Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2 };
		FVector<4> Inv1{ Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4 };
		FVector<4> Inv2{ Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5 };
		FVector<4> Inv3{ Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5 };

		FVector<4> SignA{ .x = 1.f, .y = -1.f, .z = 1.f, .w = -1.f };
		FVector<4> SignB{ .x = -1.f, .y = 1.f, .z = -1.f, .w = 1.f };
		FMatrix<4, 4> Inverse{ Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB };

		FVector<4> Row0{ .x = Inverse[0].x, .y = Inverse[1].x, .z = Inverse[2].x, .w = Inverse[3].x };

		FVector<4> Dot0(matrix[0] * Row0);
		f32 Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		f32 OneOverDeterminant = 1.f / Dot1;

		return Inverse.Scale(OneOverDeterminant);
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