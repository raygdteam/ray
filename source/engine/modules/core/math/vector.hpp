#pragma once
#include <core/core.hpp>

// 'F' stands for 'fast'
template<u8 Rank>
struct FVector;

template<>
struct FVector<1>
{
	f32 x = 0;

	/**
	 * Scalar multiplication
	 */
	FVector<1> CCFASTCALL Multiply(f32 scalar)
	{
		return FVector<1> {
			.x = this->x * scalar,
		};
	}

	/**
	 * Vector addition.
	 * https://mathworld.wolfram.com/VectorAddition.html
	 */
	FVector<1> CCFASTCALL Add(const FVector<1>& vec)
	{
		return FVector<1> {
			.x = this->x + vec.x,
		};
	}

	/**
	 * Vector dot(scalar) product.
	 * (a[0] * b[0]) + (a[1] * b[1]) + ... + (a[n-1] * b[n-1])
	 */
	f64 CCFASTCALL DotProduct(const FVector<1>& vec)
	{
		f64 product = 0;

		product += this->x * vec.x;

		return product;
	}
};

template<>
struct FVector<2>
{
	f32 x = 0;
	f32 y = 0;

	/**
	 * Scalar multiplication
	 */
	FVector<2> CCFASTCALL Multiply(f32 scalar)
	{
		return FVector<2> {
			.x = this->x * scalar,
			.y = this->y * scalar
		};
	}

	/**
	 * Vector addition.
	 * https://mathworld.wolfram.com/VectorAddition.html
	 */
	FVector<2> CCFASTCALL Add(const FVector<2>& vec)
	{
		return FVector<2> {
			.x = this->x + vec.x,
			.y = this->x + vec.y,
		};
	}

	/**
	 * Vector dot(scalar) product.
	 * (a[0] * b[0]) + (a[1] * b[1]) + ... + (a[n-1] * b[n-1])
	 */
	f64 CCFASTCALL DotProduct(const FVector<2>& vec)
	{
		f64 product = 0;

		product += this->x * vec.x;
		product += this->y * vec.y;

		return product;
	}
};

template<>
struct FVector<3>
{
	f32 x = 0;
	f32 y = 0;
	f32 z = 0;

	/**
	 * Scalar multiplication
	 */
	FVector<3> Multiply(f32 scalar)
	{
		return FVector<3> {
			.x = this->x * scalar,
			.y = this->y * scalar,
			.z = this->z * scalar
		};
	}

	/**
	 * Vector addition.
	 * https://mathworld.wolfram.com/VectorAddition.html
	 */
	FVector<3> CCFASTCALL Add(const FVector<3>& vec)
	{
		return FVector<3> {
			.x = this->x + vec.x,
			.y = this->x + vec.y,
			.z = this->z + vec.z
		};
	}

	/**
	 * Vector dot(scalar) product.
	 * (a[0] * b[0]) + (a[1] * b[1]) + ... + (a[n-1] * b[n-1])
	 */
	f64 CCFASTCALL DotProduct(const FVector<3>& vec)
	{
		f64 product = 0;

		product += this->x * vec.x;
		product += this->y * vec.y;
		product += this->z * vec.z;

		return product;
	}
};

template<>
struct FVector<4>
{
	f32 x = 0;
	f32 y = 0;
	f32 z = 0;
	f32 w = 0;

	/**
	 * Scalar multiplication
	 */
	FVector<4> Multiply(f32 scalar)
	{
		return FVector<4> {
			.x = this->x * scalar,
			.y = this->y * scalar,
			.z = this->z * scalar,
			.w = this->w * scalar,
		};
	}

	/**
	 * Vector addition.
	 * https://mathworld.wolfram.com/VectorAddition.html
	 */
	FVector<4> CCFASTCALL Add(const FVector<4>& vec)
	{
		return FVector<4> {
			.x = this->x + vec.x,
			.y = this->x + vec.y,
			.z = this->z + vec.z,
			.w = this->w + vec.w,
		};
	}

	/**
	 * Vector dot(scalar) product.
	 * (a[0] * b[0]) + (a[1] * b[1]) + ... + (a[n-1] * b[n-1])
	 */
	f64 CCFASTCALL DotProduct(const FVector<4>& vec)
	{
		f64 product = 0;

		product += this->x * vec.x;
		product += this->y * vec.y;
		product += this->z * vec.z;
		product += this->w * vec.w;

		return product;
	}
};
