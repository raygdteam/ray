#pragma once

#include <core/core.hpp>

namespace ray::math
{
	template<size_t n, class type>
	struct RAY_CORE_API Vector
	{
	public:
		Vector()
		{
			this->_data = new type[n];
		}

		template <typename ... Arguments>
		Vector(Arguments ... arguments)
		{
			this->_data = new type[n]{ arguments ... };
		}

		Vector(const Vector& other)
		{
			this->_data = other._data;
		}

		Vector& add(const Vector& other)
		{
			for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
				this->_data[i] += other._data[i];

			return this;
		}

		Vector& subtract(const Vector& other)
		{
			for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
				this->_data[i] -= other._data[i];

			return this;
		}

		Vector& multiply(const Vector& other)
		{
			for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
				this->_data[i] *= other._data[i];

			return this;
		}

		Vector& divide(const Vector& other)
		{
			for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
				this->_data[i] /= other._data[i];

			return this;
		}

		friend Vector operator+(Vector left, const Vector& right)
		{
			return left.add(right);
		}

		friend Vector operator-(Vector left, const Vector& right)
		{
			return left.subtract(right);
		}

		friend Vector operator*(Vector left, const Vector& right)
		{
			return left.multiply(right);
		}

		friend Vector operator/(Vector left, const Vector& right)
		{
			return left.divide(right);
		}

		Vector& operator+=(const Vector& other)
		{
			return add(other);
		}

		Vector& operator-=(const Vector& other)
		{
			return subtract(other);
		}

		Vector& operator*=(const Vector& other)
		{
			return multiply(other);
		}

		Vector& operator/=(const Vector& other)
		{
			return divide(other);
		}

		bool operator==(const Vector& other)
		{
			/*return (x == other.x && y == other.y); */
				/* I'm too lazy to come up with a quick way to compare two vectors. */

			return false;
		}

		bool operator!=(const Vector& other)
		{
			return !(this == other);
		}

		type* _data;
	};
}