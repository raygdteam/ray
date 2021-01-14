#pragma once
#include <core/core.hpp>

template<size_t Rank, typename Type>
struct RAY_CORE_API Vector
{
	Type* _data;
public:
	Vector()
	{
		this->_data = new Type[Rank];
	}

	template <typename... Arguments>
	Vector(Arguments... arguments)
	{
		this->_data = new Type[Rank]{ arguments ... };
	}

	Vector(const Vector& other)
	{
		this->_data = other._data;
	}

	Vector& Add(const Vector& other)
	{
		for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
			this->_data[i] += other._data[i];

		return this;
	}

	Vector& Subtract(const Vector& other)
	{
		for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
			this->_data[i] -= other._data[i];

		return this;
	}

	Vector& Multiply(const Vector& other)
	{
		for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
			this->_data[i] *= other._data[i];

		return this;
	}

	Vector& Divide(const Vector& other)
	{
		for (auto i = 0; i < sizeof(this->_data) / sizeof(this->_data[0]); i++)
			this->_data[i] /= other._data[i];

		return this;
	}

	friend Vector operator+(Vector left, const Vector& right)
	{
		return left.Add(right);
	}

	friend Vector operator-(Vector left, const Vector& right)
	{
		return left.Subtract(right);
	}

	friend Vector operator*(Vector left, const Vector& right)
	{
		return left.Multiply(right);
	}

	friend Vector operator/(Vector left, const Vector& right)
	{
		return left.Divide(right);
	}

	Vector& operator+=(const Vector& other)
	{
		return Add(other);
	}

	Vector& operator-=(const Vector& other)
	{
		return Subtract(other);
	}

	Vector& operator*=(const Vector& other)
	{
		return Multiply(other);
	}

	Vector& operator/=(const Vector& other)
	{
		return Divide(other);
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
};