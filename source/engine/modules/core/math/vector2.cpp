#include "vector2.hpp"

namespace ray::math
{

Vector2& Vector2::add(const Vector2& other)
{
    x += other.x;
    y += other.y;

    return *this;
}

Vector2& Vector2::subtract(const Vector2& other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

Vector2& Vector2::multiply(const Vector2& other)
{
    x *= other.x;
    y *= other.y;

    return *this;
}

Vector2& Vector2::divide(const Vector2& other)
{
    x /= other.x;
    y /= other.y;

    return *this;
}

Vector2 operator+(Vector2 left, const Vector2& right)
{
    return left.add(right);
}

Vector2 operator-(Vector2 left, const Vector2& right)
{
    return left.subtract(right);
}

Vector2 operator*(Vector2 left, const Vector2& right)
{
    return left.multiply(right);
}

Vector2 operator/(Vector2 left, const Vector2& right)
{
    return left.divide(right);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    return add(other);
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    return subtract(other);
}

Vector2& Vector2::operator*=(const Vector2& other)
{
    return multiply(other);
}

Vector2& Vector2::operator/=(const Vector2& other)
{
    return divide(other);
}

bool Vector2::operator==(const Vector2& other)
{
    return (x == other.x && y == other.y);
}

bool Vector2::operator!=(const Vector2& other)
{
    return !(*this == other);
}
}