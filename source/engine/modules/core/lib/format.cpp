#include "format.hpp"

#include <string.h> /* strlen, strcpy */

char _format(int variable)
{
	return 'd';
}

char _format(char variable)
{
	return 'c';
}

char _format(const char* variable)
{
	return 's';
}

template<typename type, typename... Arguments>
const char* _format(const char* format, char* buffer, size_t& iteration, type argument)
{
	for (; *format != '\0'; format++)
	{
		if (buffer[iteration] == '{' && buffer[iteration + 1] == '}')
		{
			buffer[iteration] = '%';
			buffer[++iteration] = _format(argument);
		}

		iteration++;
	}

	return buffer;
}

template<typename type, typename... Arguments>
const char* _format(const char* format, char* buffer, size_t& iteration, type argument, Arguments ... arguments)
{
	for (; *format != '\0'; format++)
	{
		if (buffer[iteration] == '{' && buffer[iteration + 1] == '}')
		{
			buffer[iteration] = '%';
			buffer[++iteration] = _format(argument);

			_format(format, buffer, iteration, arguments...);

			return buffer;
		}

		iteration++;
	}

	return buffer;
}

const char* format(const char* format)
{
	return format;
}

template<typename type>
const char* format(const char* format, type argument)
{
	char* buffer = new char[strlen(format)];

	strcpy(buffer, format);

	size_t iteration = 0;

	for (; *format != '\0'; format++)
	{
		if (buffer[iteration] == '{' && buffer[iteration + 1] == '}')
		{
			buffer[iteration] = '%';
			buffer[++iteration] = _format(argument);

			break;
		}

		iteration++;
	}

	return buffer;
}

template<typename type, typename ...Arguments>
const char* format(const char* format, type argument, Arguments ...arguments)
{
	char* buffer = new char[strlen(format)];

	strcpy(buffer, format);

	size_t iteration = 0;

	for (; *format != '\0'; format++)
	{
		if (buffer[iteration] == '{' && buffer[iteration + 1] == '}')
		{
			buffer[iteration] = '%';
			buffer[++iteration] = _format(argument);

			_format(format, buffer, iteration, arguments...);
		}

		iteration++;
	}

	return buffer;
}