#pragma once
#include "Iterator.hpp"

namespace raytl
{

template<typename StringType = char>
class String 
{
	StringType* data;
	size_t size;
public:
	String(const StringType* data);

	String(size_t init_size = 20);

	String(const String& string);

	~String();

	StringType* GetText();

	void SetText(const char* text);

	/**
	* Returns the current string length (\0 is the end of the string)
	*/
	size_t GetLength() const;

	/**
	* Set the length of the string
	* If the new length is smaller than its data, everything beyond will be cur
	* If the new length is bigger, all elements will be initialized with \0
	*/
	void SetLength(size_t size);

	void Add(const String& text);

	char Get(size_t pos) const;

	bool Compare(const String& string) const;

	/** Operators **/

	char operator[](size_t pos);

	/** Iterator **/

	Iterator<char> begin() {
		Iterator<char> it(data);
		return it;
	}

	Iterator<char> end()
	{
		Iterator<char> it(data + size);
		return it;
	}
};


}