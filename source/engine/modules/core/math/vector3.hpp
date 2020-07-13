#pragma once
#include <core/core.hpp>

namespace ray::math
{
	
struct RAY_CORE_API Vector3
{
	Vector3()
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(const Vector3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	Vector3& add(const Vector3& other);
	Vector3& subtract(const Vector3& other);
	Vector3& multiply(const Vector3& other);
	Vector3& divide(const Vector3& other);

	friend Vector3 operator+(Vector3 left, const Vector3& right);
	friend Vector3 operator-(Vector3 left, const Vector3& right);
	friend Vector3 operator*(Vector3 left, const Vector3& right);
	friend Vector3 operator/(Vector3 left, const Vector3& right);

	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(const Vector3& other);
	Vector3& operator/=(const Vector3& other);

	bool operator==(const Vector3& other);
	bool operator!=(const Vector3& other);

	float x, y, z;
};
	
}