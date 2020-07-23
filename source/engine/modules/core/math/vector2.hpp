#pragma once
#include <core/core.hpp>

namespace ray::math
{
	
struct RAY_CORE_API Vector2
{
	Vector2()
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2& other)
	{
		this->x = other.x;
		this->y = other.y;
	}

	Vector2& add(const Vector2& other);
	Vector2& subtract(const Vector2& other);
	Vector2& multiply(const Vector2& other);
	Vector2& divide(const Vector2& other);

	friend Vector2 operator+(Vector2 left, const Vector2& right);
	friend Vector2 operator-(Vector2 left, const Vector2& right);
	friend Vector2 operator*(Vector2 left, const Vector2& right);
	friend Vector2 operator/(Vector2 left, const Vector2& right);

	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(const Vector2& other);
	Vector2& operator/=(const Vector2& other);

	bool operator==(const Vector2& other);
	bool operator!=(const Vector2& other);

	float x, y;
};
	
}