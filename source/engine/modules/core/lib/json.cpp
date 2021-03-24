#include "json.hpp"

ray::json::dictionary* ray::json::parser::dictionary(int& fiterator, long size, char** tokens, ray::json::parser::type* types)
{
	auto dictionary = new ray::json::dictionary;

	for (fiterator++; fiterator < size; fiterator++)
	{
	label:
		switch (types[fiterator])
		{
		case ray::json::parser::type::key: {
			auto key = tokens[fiterator++];
			switch (types[++fiterator])
			{
			case ray::json::parser::type::value:
				//rewrite this
				if (String(tokens[fiterator]) == String("true"))
					dictionary->operator[](key) = true;
				else if (String(tokens[fiterator]) == String("true"))
					dictionary->operator[](key) = true;
				else
					if (tokens[fiterator][0] == '-')
					{
						for (auto siterator = 1; tokens[fiterator][siterator]; siterator++)
							if (isdigit(tokens[fiterator][siterator]) == 0)
							{
								dictionary->operator[](key) = tokens[fiterator++];
								goto label;
							}
						dictionary->operator[](key) = atoi(tokens[fiterator]);
					}
					else
					{
						for (auto siterator = 0; tokens[fiterator][siterator]; siterator++)
							if (isdigit(tokens[fiterator][siterator]) == 0)
							{
								dictionary->operator[](key) = tokens[fiterator++];
								goto label;
							}
						dictionary->operator[](key) = atoi(tokens[fiterator]);
					}
				//rewrite this
				break;
			case ray::json::parser::type::osbraces:
				dictionary->operator[](key) = ray::json::parser::array(fiterator, size, tokens, types);
				break;
			case ray::json::parser::type::ocbraces:
				dictionary->operator[](key) = ray::json::parser::dictionary(fiterator, size, tokens, types);
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

ray::json::value ray::json::parser::array(int& iterator, long size, char** tokens, ray::json::parser::type* types)
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

ray::json::dictionary ray::json::parse(const char* result)
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