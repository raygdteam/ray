#pragma once

#include "core/lib/array.hpp"
#include "core/lib/string.hpp"

#include <eastl/map.h>

namespace ray::json
{
	enum class value_type
	{
		none,
		string,
		integer,
		array,
		dictionary
	};

	class dictionary;

	class value
	{
		union {
			char* _string_value;
			int _integer_value;
			bool _boolean_value;
			Array<value>* _array_value;//rewrite
			dictionary* _dictionary_value;
		};

		value_type _type;
	public:
		void operator=(char* value)
		{
			this->_type = value_type::string;
			this->_string_value = value;
		}

		void operator=(int value)
		{
			this->_type = value_type::integer;
			this->_integer_value = value;
		}

		void operator=(bool value)
		{
			this->_type = value_type::integer;
			this->_boolean_value = value;
		}

		void operator=(Array<value>* value)
		{
			this->_type = value_type::array;
			this->_array_value = value;
		}

		void operator=(dictionary* value)
		{
			this->_type = value_type::dictionary;
			this->_dictionary_value = value;
		}

		char* as_string()
		{
			//assert
			return this->_string_value;
		}

		bool as_boolean()
		{
			//assert
			return this->_boolean_value;
		}

		int as_integer()
		{
			//assert
			return this->_integer_value;
		}

		Array<value>* as_array()
		{
			//assert
			return this->_array_value;
		}

		dictionary* as_dictionary()
		{
			//assert
			return this->_dictionary_value;
		}
	};

	class dictionary
	{
		eastl::map<String, value> _unnamed;
	public:
		dictionary() {}
		~dictionary() {}

		dictionary(dictionary* dictionary)
		{
			this->_unnamed = dictionary->_unnamed;
		}

		value& operator[](const char* key)
		{
			return this->_unnamed[String(key)];
		}
	};

	namespace parser
	{
		enum class type
		{
			none,
			ocbraces,
			ccbraces,
			osbraces,
			csbraces,
			key,
			value,
			colon,
			comma
		};

		ray::json::value array(int&, long, char**, ray::json::parser::type*);
		ray::json::dictionary* dictionary(int&, long, char**, ray::json::parser::type*);
	}

	ray::json::dictionary __declspec(dllexport) parse(const char*);
}