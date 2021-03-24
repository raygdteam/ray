#pragma once

/*#include "core/lib/array.hpp"
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

		extern ray::json::value array(int& iterator, long size, char** tokens, ray::json::parser::type* types);

		ray::json::dictionary* dictionary(int& iterator, long size, char** tokens, ray::json::parser::type* types)
		{
			auto dictionary = new ray::json::dictionary;

			for (iterator++; iterator < size; iterator++)
			{
				switch (types[iterator])
				{
				case ray::json::parser::type::key: {
					auto key = tokens[iterator++];
					switch (types[++iterator])
					{
					case ray::json::parser::type::value:
						//rewrite this
						if (String(tokens[iterator]) == String("true"))
							dictionary->operator[](key) = true;
						else if (String(tokens[iterator]) == String("true"))
							dictionary->operator[](key) = true;
						else
							dictionary->operator[](key) = tokens[iterator];
						//rewrite this
						break;
					case ray::json::parser::type::osbraces:
						dictionary->operator[](key) = ray::json::parser::array(iterator, size, tokens, types);
						break;
					case ray::json::parser::type::ocbraces:
						dictionary->operator[](key) = ray::json::parser::dictionary(iterator, size, tokens, types);
						break;
					default:
						//error
						break;
					}
					break;
				}
				case ray::json::parser::type::ccbraces:
					return dictionary;
					break;
				default:
					//error
					break;
				}
			}

			return dictionary;
		}

		ray::json::value array(int& iterator, long size, char** tokens, ray::json::parser::type* types)
		{
			ray::json::value fvalue;
			fvalue = new Array<ray::json::value>;

			for (iterator++; iterator < size; iterator++)
			{
				switch (types[iterator])
				{
				case ray::json::parser::type::ocbraces:
					ray::json::value svalue;
					svalue = ray::json::parser::dictionary(iterator, size, tokens, types);
					fvalue.as_array()->push_back(svalue);
					break;
				case ray::json::parser::type::csbraces:
					return fvalue;
					break;
				default:
					//error
					break;
				}
			}

			return fvalue;
		}
	}

	ray::json::dictionary parse(const char* result)
	{
		auto size = 0;

		for (auto iterator = 0; result[iterator]; iterator++)
		{
		flabel:
			switch (result[iterator])
			{
			case 34:
				if (result[++iterator] == 34)
					;//error
				else {
					for (iterator++; result[iterator]; iterator++)
					{
						switch (result[iterator])
						{
						case 34:
							size++; iterator++;
							goto flabel;
						default:
							break;
						}
					}
				}
				break;
			case 44: case 58: case 91: case 93: case 123: case 125:
				size++; break;
			case 9: case 10: case 32:
				break;
			default:
				for (iterator++; result[iterator]; iterator++)
				{
					switch (result[iterator])
					{
					case 9: case 10: case 32: case 44:
						size++; iterator++;
						goto flabel;
					default:
						break;
					}
				}
				break;
			}
		}

		auto tokens_range = new int[size][2];
		auto tokens_type = new ray::json::parser::type[size];

		for (auto fiterator = 0, siterator = 0, titerator = 0; result[fiterator]; fiterator++)
		{
		slabel:
			switch (result[fiterator])
			{
			case 34:
				tokens_range[siterator][0] = ++fiterator;
				for (fiterator++; result[fiterator]; fiterator++)
				{
					switch (result[fiterator])
					{
					case 34:
						tokens_type[titerator++] = tokens_type[titerator - 2] == ray::json::parser::type::key ? ray::json::parser::type::value : ray::json::parser::type::key;
						tokens_range[siterator++][1] = fiterator - 1;
						fiterator++; goto slabel;
					default:
						break;
					}
				}
				break;
			case 44:
				tokens_range[siterator][0] = fiterator;
				tokens_range[siterator++][1] = fiterator;
				tokens_type[titerator++] = ray::json::parser::type::comma;
				break;
			case 58:
				tokens_range[siterator][0] = fiterator;
				tokens_range[siterator++][1] = fiterator;
				tokens_type[titerator++] = ray::json::parser::type::colon;
				break;
			case 91:
				tokens_range[siterator][0] = fiterator;
				tokens_range[siterator++][1] = fiterator;
				tokens_type[titerator++] = ray::json::parser::type::osbraces;
				break;
			case 93:
				tokens_range[siterator][0] = fiterator;
				tokens_range[siterator++][1] = fiterator;
				tokens_type[titerator++] = ray::json::parser::type::csbraces;
				break;
			case 123:
				tokens_range[siterator][0] = fiterator;
				tokens_range[siterator++][1] = fiterator;
				tokens_type[titerator++] = ray::json::parser::type::ocbraces;
				break;
			case 125:
				tokens_range[siterator][0] = fiterator;
				tokens_range[siterator++][1] = fiterator;
				tokens_type[titerator++] = ray::json::parser::type::ccbraces;
				break;
			case 9: case 10: case 32:
				break;
			default:
				tokens_range[siterator][0] = fiterator;
				for (fiterator++; result[fiterator]; fiterator++)
				{
					switch (result[fiterator])
					{
					case 9: case 10: case 32: case 44:
						if (tokens_type[titerator - 2] == ray::json::parser::type::key)
							tokens_type[titerator++] = ray::json::parser::type::value;
						else
							;//error
						tokens_range[siterator++][1] = fiterator - 1;
						fiterator++; goto slabel;
					default:
						break;
					}
				}
				break;
			}
		}

		auto tokens_buffer = new char* [size * sizeof(char*)];

		for (auto fiterator = 0, siterator = 0; fiterator < size; fiterator++, siterator = 0)
		{
			tokens_buffer[fiterator] = new char[(tokens_range[fiterator][1] - tokens_range[fiterator][0]) + 2 * sizeof(char)];
			for (auto titerator = tokens_range[fiterator][0]; titerator < tokens_range[fiterator][1] + 1; titerator++, siterator++)
				tokens_buffer[fiterator][siterator] = result[titerator];
			tokens_buffer[fiterator][siterator] = 0;
		}

		int iterator = 0;
		return ray::json::parser::dictionary(iterator, size, tokens_buffer, tokens_type);
	}
}*/