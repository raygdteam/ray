#pragma once

#include "core/lib/array.hpp"
#include "core/lib/string.hpp"

#include <eastl/map.h>

enum DictionaryValueType
{
	eDictionaryValueNone,
	eDictionaryValueString,
	eDictionaryValueObject,
	eDictionaryValueArray,
	eDictionaryValueBoolean,
	eDictionaryValueInteger
};

class RAY_CORE_API DictionaryObject;

class RAY_CORE_API DictionaryValue
{
	union {
		char* _string_value;
		int _integer_value;
		bool _boolean_value;
		Array<DictionaryValue>* _array_value;
		DictionaryObject* _object_value;
	};

	DictionaryValueType _type;
public:
	void operator=(char* value)
	{
		this->_type = eDictionaryValueString;
		this->_string_value = value;
	}

	void operator=(int value)
	{
		this->_type = eDictionaryValueInteger;
		this->_integer_value = value;
	}

	void operator=(bool value)
	{
		this->_type = eDictionaryValueBoolean;
		this->_boolean_value = value;
	}

	void operator=(Array<DictionaryValue>* value)
	{
		this->_type = eDictionaryValueArray;
		this->_array_value = value;
	}

	void operator=(DictionaryObject* value)
	{
		this->_type = eDictionaryValueObject;
		this->_object_value = value;
	}

	char* AsString()
	{
		//assert
		return this->_string_value;
	}

	bool AsBoolean()
	{
		//assert
		return this->_boolean_value;
	}

	int AsInteger()
	{
		//assert
		return this->_integer_value;
	}

	Array<DictionaryValue>& AsArray()
	{
		//assert
		return *this->_array_value;
	}

	DictionaryObject& AsObject()
	{
		//assert
		return *this->_object_value;
	}
};

class RAY_CORE_API DictionaryObject
{
	eastl::map<String, DictionaryValue> _map;
public:
	DictionaryObject() {}
	~DictionaryObject() {}

	DictionaryObject(DictionaryObject* dictionary)
	{
		this->_map = dictionary->_map;
	}

	DictionaryValue& operator[](const char* key)
	{
		return this->_map[String(key)];
	}
};

enum class DictionaryParserValueType
{
	eDictionaryParserValueTypeNone,
	eDictionaryParserValueTypeOCBraces,
	eDictionaryParserValueTypeCCBraces,
	eDictionaryParserValueTypeOSBraces,
	eDictionaryParserValueTypeCSBraces,
	eDictionaryParserValueTypeKey,
	eDictionaryParserValueTypeValue,
	eDictionaryParserValueTypeColon,
	eDictionaryParserValueTypeComma
};

DictionaryValue DictionaryParseArray(int&, long, char**, DictionaryParserValueType*);
DictionaryObject* DictionaryParseObject(int&, long, char**, DictionaryParserValueType*);

DictionaryObject RAY_CORE_API DictionaryParse(const char*);