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

	FMatrix(FVector4 row0 = FVector4(), FVector4 row1 = FVector4(), FVector4 row2 = FVector4(), FVector4 row3 = FVector4())
	{
		Row[0] = FVector4 { .x = row0.x, .y = row0.y, .z = row0.z, .w = row0.w };
		Row[1] = FVector4 { .x = row1.x, .y = row1.y, .z = row1.z, .w = row1.w };
		Row[2] = FVector4 { .x = row2.x, .y = row2.y, .z = row2.z, .w = row2.w };
		Row[3] = FVector4 { .x = row3.x, .y = row3.y, .z = row3.z, .w = row3.w };
	}

	static FMatrix Identity()
	{
		return FMatrix(
			{ 1.f, 0.0f, 0.0f, 0.f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.f, 0.0f, 1.0f, 0.f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		);
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

	FMatrix<4, 4> Multiply(f32 scalar) const
	{
		// TODO: nice-ify
		FVector<4> A[4] = { Row[0], Row[1], Row[2], Row[3] };
		
		return FMatrix<4, 4> {
			{
				(A[0].x * scalar),
				(A[0].y * scalar),
				(A[0].z * scalar),
				(A[0].w * scalar)
			},
			{
				(A[1].x * scalar),
				(A[1].y * scalar),
				(A[1].z * scalar),
				(A[1].w * scalar)
			},
			{
				(A[2].x * scalar),
				(A[2].y * scalar),
				(A[2].z * scalar),
				(A[3].w * scalar)
			},
			{
				(A[3].x * scalar),
				(A[3].y * scalar),
				(A[3].z * scalar),
				(A[3].w * scalar)
			},
		};

	}
	
	FMatrix<4, 4> Multiply(const FMatrix& param) const 
	{
		FMatrix<4, 4> result;
		
		float x = Row[0].x;
		float y = Row[0].y;
		float z = Row[0].z;
		float w = Row[0].w;
		
		result.Row[0].x = (param.Row[0].x * x) + (param.Row[1].x * y) + (param.Row[2].x * z) + (param.Row[3].x * w);
		result.Row[0].y = (param.Row[0].y * x) + (param.Row[1].y * y) + (param.Row[2].y * z) + (param.Row[3].y * w);
		result.Row[0].z = (param.Row[0].z * x) + (param.Row[1].z * y) + (param.Row[2].z * z) + (param.Row[3].z * w);
		result.Row[0].w = (param.Row[0].w * x) + (param.Row[1].w * y) + (param.Row[2].w * z) + (param.Row[3].w * w);

		x = Row[1].x;
		y = Row[1].y;
		z = Row[1].z;
		w = Row[1].w;
		
		result.Row[1].x = (param.Row[0].x * x) + (param.Row[1].x * y) + (param.Row[2].x * z) + (param.Row[3].x * w);
		result.Row[1].y = (param.Row[0].y * x) + (param.Row[1].y * y) + (param.Row[2].y * z) + (param.Row[3].y * w);
		result.Row[1].z = (param.Row[0].z * x) + (param.Row[1].z * y) + (param.Row[2].z * z) + (param.Row[3].z * w);
		result.Row[1].w = (param.Row[0].w * x) + (param.Row[1].w * y) + (param.Row[2].w * z) + (param.Row[3].w * w);
		
		x = Row[2].x;
		y = Row[2].y;
		z = Row[2].z;
		w = Row[2].w;
		
		result.Row[2].x = (param.Row[0].x * x) + (param.Row[1].x * y) + (param.Row[2].x * z) + (param.Row[3].x * w);
		result.Row[2].y = (param.Row[0].y * x) + (param.Row[1].y * y) + (param.Row[2].y * z) + (param.Row[3].y * w);
		result.Row[2].z = (param.Row[0].z * x) + (param.Row[1].z * y) + (param.Row[2].z * z) + (param.Row[3].z * w);
		result.Row[2].w = (param.Row[0].w * x) + (param.Row[1].w * y) + (param.Row[2].w * z) + (param.Row[3].w * w);
		
		x = Row[3].x;
		y = Row[3].y;
		z = Row[3].z;
		w = Row[3].w;
		
		result.Row[3].x = (param.Row[0].x * x) + (param.Row[1].x * y) + (param.Row[2].x * z) + (param.Row[3].x * w);
		result.Row[3].y = (param.Row[0].y * x) + (param.Row[1].y * y) + (param.Row[2].y * z) + (param.Row[3].y * w);
		result.Row[3].z = (param.Row[0].z * x) + (param.Row[1].z * y) + (param.Row[2].z * z) + (param.Row[3].z * w);
		result.Row[3].w = (param.Row[0].w * x) + (param.Row[1].w * y) + (param.Row[2].w * z) + (param.Row[3].w * w);
		
		return result;

	}

	FMatrix<4, 4> Translate(FVector<3> const& vector)
	{
		Row[3] = Row[0].Multiply(vector.x)
			+ Row[1].Multiply(vector.y)
			+ Row[2].Multiply(vector.z) + Row[3];

		return FMatrix<4, 4>(*this);
	}

	static FMatrix<4, 4> Inverse(FMatrix<4, 4> matrix)
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

		return inverse.Multiply(oneOverDeterminant);
	}

	FMatrix<4, 4> Inverse()
	{
		FMatrix<4, 4> matrix = *this;
		
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

		FVector<4> fac0{ .x = coef00, .y = coef00, .z = coef02, .w = coef03 };
		FVector<4> fac1{ .x = coef04, .y = coef04, .z = coef06, .w = coef07 };
		FVector<4> fac2{ .x = coef08, .y = coef08, .z = coef10, .w = coef11 };
		FVector<4> fac3{ .x = coef12, .y = coef12, .z = coef14, .w = coef15 };
		FVector<4> fac4{ .x = coef16, .y = coef16, .z = coef18, .w = coef19 };
		FVector<4> fac5{ .x = coef20, .y = coef20, .z = coef22, .w = coef23 };

		FVector<4> vec0{ .x = matrix.Row[1].x, .y = matrix.Row[0].x, .z = matrix.Row[0].x, .w = matrix.Row[0].x };
		FVector<4> vec1{ .x = matrix.Row[1].y, .y = matrix.Row[0].y, .z = matrix.Row[0].y, .w = matrix.Row[0].y };
		FVector<4> vec2{ .x = matrix.Row[1].z, .y = matrix.Row[0].z, .z = matrix.Row[0].z, .w = matrix.Row[0].z };
		FVector<4> vec3{ .x = matrix.Row[1].w, .y = matrix.Row[0].w, .z = matrix.Row[0].w, .w = matrix.Row[0].w };

		FVector<4> inv0{ vec1 * fac0 - vec2 * fac1 + vec3 * fac2 };
		FVector<4> inv1{ vec0 * fac0 - vec2 * fac3 + vec3 * fac4 };
		FVector<4> inv2{ vec0 * fac1 - vec1 * fac3 + vec3 * fac5 };
		FVector<4> inv3{ vec0 * fac2 - vec1 * fac4 + vec2 * fac5 };

		FVector<4> signA{ .x = 1.f, .y = -1.f, .z = 1.f, .w = -1.f };
		FVector<4> signB{ .x = -1.f, .y = 1.f, .z = -1.f, .w = 1.f };
		FMatrix<4, 4> inverse{ inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB };

		FVector<4> row0{ .x = inverse.Row[0].x, .y = inverse.Row[1].x, .z = inverse.Row[2].x, .w = inverse.Row[3].x };

		FVector<4> dot0(matrix.Row[0] * row0);
		f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

		f32 oneOverDeterminant = 1.f / dot1;

		return inverse.Multiply(oneOverDeterminant);
	}

	FMatrix<4, 4> operator=(const FMatrix<4, 4>& other)
	{
		if (this == &other)
			return *this;

		this->Row[0] = other.Row[0];
		this->Row[1] = other.Row[1];
		this->Row[2] = other.Row[2];
		this->Row[3] = other.Row[3];
		
		return *this;
	}

	FMatrix<4, 4>& operator=(FMatrix<4, 4>&& other) noexcept
	{
		if (this == &other)
			return *this;

		this->Row[0] = other.Row[0];
		this->Row[1] = other.Row[1];
		this->Row[2] = other.Row[2];
		this->Row[3] = other.Row[3];
		
		return *this;
	}

	static FMatrix<4, 4> RotationX(f32 angle)
	{
		/* TODO: get rid of it */
		using namespace ray::core::math;

		/*
		 *      X Rotation
		 * 
		 * [1,      0,       0, 0]
		 * [0, cos(α), -sin(α), 0]
		 * [0, sin(α),  cos(α), 0]
		 * [0,      0,       0, 1]
		 */
		return FMatrix(
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.f, Cos(angle), -Sin(angle), 0.f },
			{ 0.f, Sin(angle), Cos(angle), 0.f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		);
	}

	static FMatrix<4, 4> RotationY(f32 angle)
	{
		/* TODO: get rid of it */
		using namespace ray::core::math;

		/*
		 *      Y Rotation
		 *
		 * [ cos(α), 0, sin(α), 0]
		 * [      0, 1,      0, 0]
		 * [-sin(α), 0, cos(α), 0]
		 * [      0, 0,      0, 1]
		 */
		return FMatrix(
			{ Cos(angle), 0.0f, Sin(angle), 0.0f },
			{ 0.f, 1, -Sin(angle), 0.f },
			{ -Sin(angle), 0.0f, Cos(angle), 0.f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		);
	}

	static FMatrix<4, 4> RotationZ(f32 angle)
	{
		/* TODO: get rid of it */
		using namespace ray::core::math;

		/*
		 *      Z Rotation
		 *
		 * [cos(α), -sin(α), 0, 0]
		 * [sin(α),  cos(α), 0, 0]
		 * [      0,      0, 1, 0]
		 * [      0,      0, 0, 1]
		*/
		return FMatrix(
			{ Cos(angle), -Sin(angle), 0.0f, 0.0f },
			{ Sin(angle), Cos(angle), 0.0f, 0.f },
			{ 0.f, 0.0f, 1.0f, 0.f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		);
	}

	static FMatrix<4, 4> Orthographic(f32 width, f32 height, float nearz, float farz)
	{
		f32 w = 2 / width;
		f32 h = 2 / height;
		f32 a = 1.f / (farz - nearz);
		f32 b = -a * nearz;

		/*
		 * [ w, 0, 0, 0]
		 * [ 0, h, 0, 0]
		 * [ 0, 0, a, 0]
		 * [ 0, 0, b, 1]
		 */

		FMatrix<4, 4> result = FMatrix(
			{ w, 0.0f, 0.0f, 0.f },
			{ 0.0f, h, 0.0f, 0.0f },
			{ 0.f, 0.0f, a, 0.f },
			{ 0.0f, 0.0f, b, 1.0f }
		);
		
		return result;
	}

	FMatrix<4, 4> Transpose()
	{
		return FMatrix<4, 4>(
			{ Row[0].x, Row[1].x, Row[2].x, Row[3].x },
			{ Row[0].y, Row[1].y, Row[2].y, Row[3].y },
			{ Row[0].z, Row[1].z, Row[2].z, Row[3].z },
			{ Row[0].w, Row[1].w, Row[2].w, Row[3].w }
		);
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

	FMatrix<4, 4> operator*(const FMatrix<4, 4>& param) const
	{
		return Multiply(param);
	}

	FMatrix<4, 4> operator*(f32 param) const
	{
		return Multiply(param);
	}

	FMatrix(const FMatrix<4, 4>& other)
	{
		this->Row[0] = other.Row[0];
		this->Row[1] = other.Row[1];
		this->Row[2] = other.Row[2];
		this->Row[3] = other.Row[3];
	}

	FMatrix(FMatrix<4, 4>&& other) noexcept
	{
		this->Row[0] = other.Row[0];
		this->Row[1] = other.Row[1];
		this->Row[2] = other.Row[2];
		this->Row[3] = other.Row[3];
	}
};

using FMatrix2 = FMatrix<2, 2>; using FMatrix2x2 = FMatrix<2, 2>;
using FMatrix3 = FMatrix<3, 3>; using FMatrix3x3 = FMatrix<3, 3>;
using FMatrix4 = FMatrix<4, 4>; using FMatrix4x4 = FMatrix<4, 4>;

/*
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
*/
