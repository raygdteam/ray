#include "pch.hpp"
#include "string.hpp"
#include "platform/platform_memory.hpp"
using namespace ray::core::platform;

namespace raytl
{
template <typename StringType>
String<StringType>::String(const StringType* data)
{
	size_t size = strlen(data);

	this->data = new StringType[size];
	this->size = size;

	if (this->data != nullptr) 
	{
		PlatformMemory::Memcpy(data, this->data, size);
	}
}

String<>::String(size_t init_size) {
	this->data = new char[init_size];
	this->size = init_size;

	if (this->data != NULL) {
		for (size_t i = 0; i < init_size; i++) {
			data[i] = '\0';
		}
	}
}



String<>::String(const String<>& string) {
	this->size = string.GetLength();
	this->data = new char[this->size];

	if (this->data != NULL) {
		strncpy(this->data, string.data, this->size);
	}
}

String<>::~String() {
	if (this->data != NULL)
		delete[] this->data;
}



char* String<>::GetText() {
	return data;
}

void String<>::SetText(const char* text) {
	delete[] this->data;

	this->size = strlen(text);
	this->data = new char[this->size];

	strncpy(this->data, text, this->size);
}

size_t String<>::GetLength() const {
	return strlen(this->data);
}

void String<>::SetLength(size_t size) {
	size_t old_length = this->GetLength();
	char* old_data = this->data;

	this->size = size;
	this->data = new char[size];

	// Copy the characters (or add \0 if there are no characters anymore).
	for (size_t i = 0; i < size; i++) {
		if (i < old_length) {
			this->data[i] = old_data[i];
		}
		else {
			this->data[i] = '\0';
		}
	}

	// Destroy the old data
	delete[] old_data;

	// Ensure that the last element is \0
	this->data[size] = '\0';
}

void String<>::Add(const String & text) {
	size_t new_size = this->size + text.size;
	SetLength(new_size);

	size_t length = GetLength();
	for (size_t i = length; i < new_size; i++) {
		this->data[i] = text.data[i - length];
	}
}

char String<>::Get(size_t pos) const {
	if (pos > GetLength()) {
		return '\0';
	}
	return data[pos];
}

bool String<>::Compare(const String & string) const {
	return strcmp(this->data, string.data) == 0;
}

char String<>::operator[](size_t pos) {
	return this->Get(pos);
}

String<> operator+(const String<>& string1, const String<>& string2) {
	String string = String(string1);
	string.Add(string2);

	return string;
}

bool operator==(const String<>& string1, const String<>& string2) {
	return string1.Compare(string2);
}

}