#include <memory>

template <typename type>
class allocator
{
public:

};

template <typename type, typename allocator = allocator<type>>
class vector
{
public:
	vector() : begin(0), end(0)
	{

	}

	~vector()
	{

	}

	void reserve(size_t index)
	{
		//type* const begin = DoAllocate(index);
	}

	void push_back(const type& value)
	{
		::new((void*)this->end++) type(value);
	}

	type& operator[](size_t index)
	{
		return *(this->begin + index);
	}

protected:
	type* begin, end;
};

void __Playground_Nikita()
{
	vector<int> vector;

	vector.reserve(1);

	vector.push_back(0);

	vector[0];
}