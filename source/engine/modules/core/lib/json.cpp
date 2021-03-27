#include "json.hpp"

DictionaryObject* DictionaryParseObject(int& fiterator, long size, char** tokens, DictionaryParserValueType* types)
{
	auto dictionary = new DictionaryObject;

	for (fiterator++; fiterator < size; fiterator++)
	{
	label:
		switch (types[fiterator])
		{
		case DictionaryParserValueType::eDictionaryParserValueTypeKey: {
			auto eDictionaryParserValueTypeKey = tokens[fiterator++];
			switch (types[++fiterator])
			{
			case DictionaryParserValueType::eDictionaryParserValueTypeValue:
				//rewrite this
				if (String(tokens[fiterator]) == String("true"))
					dictionary->operator[](eDictionaryParserValueTypeKey) = true;
				else if (String(tokens[fiterator]) == String("true"))
					dictionary->operator[](eDictionaryParserValueTypeKey) = true;
				else
					if (tokens[fiterator][0] == '-')
					{
						for (auto siterator = 1; tokens[fiterator][siterator]; siterator++)
							if (isdigit(tokens[fiterator][siterator]) == 0)
							{
								dictionary->operator[](eDictionaryParserValueTypeKey) = tokens[fiterator++];
								goto label;
							}
						dictionary->operator[](eDictionaryParserValueTypeKey) = atoi(tokens[fiterator]);
					}
					else
					{
						for (auto siterator = 0; tokens[fiterator][siterator]; siterator++)
							if (isdigit(tokens[fiterator][siterator]) == 0)
							{
								dictionary->operator[](eDictionaryParserValueTypeKey) = tokens[fiterator++];
								goto label;
							}
						dictionary->operator[](eDictionaryParserValueTypeKey) = atoi(tokens[fiterator]);
					}
				//rewrite this
				break;
			case DictionaryParserValueType::eDictionaryParserValueTypeOSBraces:
				dictionary->operator[](eDictionaryParserValueTypeKey) = DictionaryParseArray(fiterator, size, tokens, types);
				break;
			case DictionaryParserValueType::eDictionaryParserValueTypeOCBraces:
				dictionary->operator[](eDictionaryParserValueTypeKey) = DictionaryParseObject(fiterator, size, tokens, types);
				break;
			default:
				//error
				break;
			}
			break;
		}
		case DictionaryParserValueType::eDictionaryParserValueTypeCCBraces:
			return dictionary;
			break;
		default:
			//error
			break;
		}
	}

	return dictionary;
}

DictionaryValue DictionaryParseArray(int& iterator, long size, char** tokens, DictionaryParserValueType* types)
{
	DictionaryValue fvalue;
	fvalue = new Array<DictionaryValue>;

	for (iterator++; iterator < size; iterator++)
	{
		switch (types[iterator])
		{
		case DictionaryParserValueType::eDictionaryParserValueTypeOCBraces:
			DictionaryValue svalue;
			svalue = DictionaryParseObject(iterator, size, tokens, types);
			fvalue.AsArray().push_back(svalue);
			break;
		case DictionaryParserValueType::eDictionaryParserValueTypeCSBraces:
			return fvalue;
			break;
		default:
			//error
			break;
		}
	}

	return fvalue;
}

DictionaryObject DictionaryParse(const char* result)
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
		case 9: case 10: case 13: case 32:
			break;
		default:
			for (iterator++; result[iterator]; iterator++)
			{
				switch (result[iterator])
				{
				case 9: case 10: case 13: case 32: case 44:
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
	auto tokens_type = new DictionaryParserValueType[size];

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
					tokens_type[titerator++] = tokens_type[titerator - 2] == DictionaryParserValueType::eDictionaryParserValueTypeKey ? DictionaryParserValueType::eDictionaryParserValueTypeValue : DictionaryParserValueType::eDictionaryParserValueTypeKey;
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
			tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeComma;
			break;
		case 58:
			tokens_range[siterator][0] = fiterator;
			tokens_range[siterator++][1] = fiterator;
			tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeColon;
			break;
		case 91:
			tokens_range[siterator][0] = fiterator;
			tokens_range[siterator++][1] = fiterator;
			tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeOSBraces;
			break;
		case 93:
			tokens_range[siterator][0] = fiterator;
			tokens_range[siterator++][1] = fiterator;
			tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeCSBraces;
			break;
		case 123:
			tokens_range[siterator][0] = fiterator;
			tokens_range[siterator++][1] = fiterator;
			tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeOCBraces;
			break;
		case 125:
			tokens_range[siterator][0] = fiterator;
			tokens_range[siterator++][1] = fiterator;
			tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeCCBraces;
			break;
		case 9: case 10: case 13: case 32:
			break;
		default:
			tokens_range[siterator][0] = fiterator;
			for (fiterator++; result[fiterator]; fiterator++)
			{
				switch (result[fiterator])
				{
				case 9: case 10: case 13: case 32: case 44:
					if (tokens_type[titerator - 2] == DictionaryParserValueType::eDictionaryParserValueTypeKey)
						tokens_type[titerator++] = DictionaryParserValueType::eDictionaryParserValueTypeValue;
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
	return DictionaryParseObject(iterator, size, tokens_buffer, tokens_type);
}