#pragma once

namespace ray::renderer_core_api::math
{
	struct Vector4
	{
		Vector4()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		}

		Vector4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->z = w;
		}

		Vector4(const Vector4& other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
		}

		Vector4& add(const Vector4& other);
		Vector4& subtract(const Vector4& other);
		Vector4& multiply(const Vector4& other);
		Vector4& divide(const Vector4& other);

		friend Vector4 operator+(Vector4 left, const Vector4& right);
		friend Vector4 operator-(Vector4 left, const Vector4& right);
		friend Vector4 operator*(Vector4 left, const Vector4& right);
		friend Vector4 operator/(Vector4 left, const Vector4& right);

		Vector4& operator+=(const Vector4& other);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator*=(const Vector4& other);
		Vector4& operator/=(const Vector4& other);

		bool operator==(const Vector4& other);
		bool operator!=(const Vector4& other);

		float x, y, z, w;
	};
}