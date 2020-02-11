#pragma once

namespace raytl
{

template<typename T>
class Iterator 
{
	T* data;
public:
	Iterator(T* data) 
	{
		this->data = data;
	}

	T& operator*() 
	{
		return *data;
	}

	Iterator<T>& operator++() 
	{
		data++;
		return *this;
	}

	bool operator==(const Iterator<T>& a) 
	{
		return data == a.data;
	}

	bool operator!=(const Iterator<T>& a) 
	{
		return data != a.data;
	}

};

}