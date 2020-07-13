#include "vector4.hpp"

namespace ray::math
{
	
Vector4& Vector4::add(const Vector4& other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->w += other.w;

    return *this;
}

Vector4& Vector4::subtract(const Vector4& other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->w -= other.w;

    return *this;
}

Vector4& Vector4::multiply(const Vector4& other)
{
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    this->w *= other.w;

    return *this;
}

Vector4& Vector4::divide(const Vector4& other)
{
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    this->w /= other.w;

    return *this;
}

Vector4 operator+(Vector4 left, const Vector4& other)
{
    return left.add(other);
}

Vector4 operator-(Vector4 left, const Vector4& other)
{
    return left.subtract(other);
}

Vector4 operator*(Vector4 left, const Vector4& other)
{
    return left.multiply(other);
}

Vector4 operator/(Vector4 left, const Vector4& other)
{
    return left.divide(other);
}

Vector4& Vector4::operator+=(const Vector4& other)
{
    return add(other);
}

Vector4& Vector4::operator-=(const Vector4& other)
{
    return subtract(other);
}

Vector4& Vector4::operator*=(const Vector4& other)
{
    return multiply(other);
}

Vector4& Vector4::operator/=(const Vector4& other)
{
    return divide(other);
}

bool Vector4::operator==(const Vector4& other)
{
    return (this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w);
}

bool Vector4::operator!=(const Vector4& other)
{
    return !(*this == other);
}
	
}
