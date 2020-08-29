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
	FVector<2> CCFASTCALL Add(const FVector<2>& vec) const
	{
		return FVector<2> {
			.x = this->x + vec.x,
			.y = this->y + vec.y,
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

	FVector<2> operator+(const FVector<2>& param) const
	{
		return Add(param);
	}

	FVector<2> operator+(FVector<2>& param) const
	{
		return Add(param);
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

	FVector<3> CCFASTCALL Multiply(const FVector<3>& vec) const
	{
		return FVector<3> {
			.x = this->x * vec.x,
			.y = this->y * vec.y,
			.z = this->z * vec.z,
		};
	}

	/**
	 * Vector addition.
	 * https://mathworld.wolfram.com/VectorAddition.html
	 */
	FVector<3> CCFASTCALL Add(const FVector<3>& vec) const
	{
		return FVector<3> {
			.x = this->x + vec.x,
			.y = this->y + vec.y,
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

	FVector<3> operator*(const FVector<3>& param) const
	{
		return Multiply(param);
	}

	FVector<3> operator*(FVector<3>& param) const
	{
		return Multiply(param);
	}

	FVector<3> operator+(const FVector<3>& param) const
	{
		return Add(param);
	}

	FVector<3> operator+(FVector<3>& param) const 
	{
		return Add(param);
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

	FVector<4> CCFASTCALL Multiply(const FVector<4>& vec) const
	{
		return FVector<4> {
			.x = this->x * vec.x,
			.y = this->y * vec.y,
			.z = this->z * vec.z,
			.w = this->w * vec.w,
		};
	}

	/**
	 * Vector addition.
	 * https://mathworld.wolfram.com/VectorAddition.html
	 */
	FVector<4> CCFASTCALL Add(const FVector<4>& vec) const
	{
		return FVector<4> {
			.x = this->x + vec.x,
			.y = this->y + vec.y,
			.z = this->z + vec.z,
			.w = this->w + vec.w,
		};
	}

	FVector<4> CCFASTCALL Subtract(const FVector<4>& vec) const
	{
		return FVector<4> {
			.x = this->x - vec.x,
			.y = this->y - vec.y,
			.z = this->z - vec.z,
			.w = this->w - vec.w,
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

	FVector<4> operator*(const FVector<4>& param) const
	{
		return Multiply(param);
	}

	FVector<4> operator*(FVector<4>& param) const
	{
		return Multiply(param);
	}

	FVector<4> operator+(const FVector<4>& param) const
	{
		return Add(param);
	}

	FVector<4> operator+(FVector<4>& param) const
	{
		return Add(param);
	}

	FVector<4> operator-(const FVector<4>& param) const
	{
		return Subtract(param);
	}

	FVector<4> operator-(FVector<4>& param) const
	{
		return Subtract(param);
	}
};

using FVector1 = FVector<1>;
using FVector2 = FVector<2>;
using FVector3 = FVector<3>;
using FVector4 = FVector<4>;

FVector<3> Normalize(FVector<3> vector)
{
	return vector.Multiply(ray::core::math::InverseSqrt(Dot(vector, vector)));
}

inline static f32 Dot(FVector<3> a, FVector<3> b)
{
	FVector3 tmp(a * b);
	return tmp.x + tmp.y + tmp.z;
}