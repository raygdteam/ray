#include "vector3.hpp"

namespace ray::renderer_core_api::math
{
    Vector3& Vector3::add(const Vector3& other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;

        return *this;
    }

    Vector3& Vector3::subtract(const Vector3& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;

        return *this;
    }

    Vector3& Vector3::multiply(const Vector3& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;

        return *this;
    }

    Vector3& Vector3::divide(const Vector3& other)
    {
        this->x /= other.x;
        this->y /= other.y;
        this->z /= other.z;

        return *this;
    }

    Vector3 operator+(Vector3 left, const Vector3& other)
    {
        return left.add(other);
    }

    Vector3 operator-(Vector3 left, const Vector3& other)
    {
        return left.subtract(other);
    }

    Vector3 operator*(Vector3 left, const Vector3& other)
    {
        return left.multiply(other);
    }

    Vector3 operator/(Vector3 left, const Vector3& other)
    {
        return left.divide(other);
    }

    Vector3& Vector3::operator+=(const Vector3& other)
    {
        return add(other);
    }

    Vector3& Vector3::operator-=(const Vector3& other)
    {
        return subtract(other);
    }

    Vector3& Vector3::operator*=(const Vector3& other)
    {
        return multiply(other);
    }

    Vector3& Vector3::operator/=(const Vector3& other)
    {
        return divide(other);
    }

    bool Vector3::operator==(const Vector3& other)
    {
        return (this->x == other.x && this->y == other.y && this->z == other.z);
    }

    bool Vector3::operator!=(const Vector3& other)
    {
        return !(*this == other);
    }
}