#pragma once

#include <core/core.hpp>

namespace ray::math
{
	template<size_t n, class type>
	struct RAY_CORE_API vector
	{
	public:
		vector()
		{
			this->data = new type[n];
		}

		template <typename ... Arguments>
		vector(Arguments ... arguments)
		{
			this->data = new type[n]{ arguments ... };
		}

		vector(const vector& other)
		{
			this->data = other.data;
		}

		vector& add(const vector& other)
		{
			for (auto i = 0; i < sizeof(this->data) / sizeof(this->data[0]); i++)
				this->data[i] += other.data[i];

			return this;
		}

		vector& subtract(const vector& other)
		{
			for (auto i = 0; i < sizeof(this->data) / sizeof(this->data[0]); i++)
				this->data[i] -= other.data[i];

			return this;
		}

		vector& multiply(const vector& other)
		{
			for (auto i = 0; i < sizeof(this->data) / sizeof(this->data[0]); i++)
				this->data[i] *= other.data[i];

			return this;
		}

		vector& divide(const vector& other)
		{
			for (auto i = 0; i < sizeof(this->data) / sizeof(this->data[0]); i++)
				this->data[i] /= other.data[i];

			return this;
		}

		friend vector operator+(vector left, const vector& right)
		{
			return left.add(right);
		}

		friend vector operator-(vector left, const vector& right)
		{
			return left.subtract(right);
		}

		friend vector operator*(vector left, const vector& right)
		{
			return left.multiply(right);
		}

		friend vector operator/(vector left, const vector& right)
		{
			return left.divide(right);
		}

		vector& operator+=(const vector& other)
		{
			return add(other);
		}

		vector& operator-=(const vector& other)
		{
			return subtract(other);
		}

		vector& operator*=(const vector& other)
		{
			return multiply(other);
		}

		vector& operator/=(const vector& other)
		{
			return divide(other);
		}

		bool operator==(const vector& other)
		{
			/*return (x == other.x && y == other.y); */
				/* I'm too lazy to come up with a quick way to compare two vectors. */

			return false;
		}

		bool operator!=(const vector& other)
		{
			return !(this == other);
		}

		type* data;
	};
}