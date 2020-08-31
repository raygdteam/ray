#pragma once

#include <string.h> /* strlen, strcpy */
#include <stdio.h> /* snprintf */


char cformat(int)
{
	return 'i';
}

char cformat(char)
{
	return 'c';
}

char cformat(const char*)
{
	return 's';
}

template<typename type>
const char* bformat(const char* format, char* buffer, int& iteration, type argument)
{
	while (*format)
	{
		if (*format != '{')
		{
			format++;
			iteration++;
			continue;
		}
		else
			format++;

		if (*format != '}')
		{
			format++;
			iteration++;
			continue;
		}
		else
		{
			buffer[iteration] = '%';
			buffer[++iteration] = cformat(argument);

			break;
		}
	}

	return buffer;
}

template<typename type, typename ... types>
const char* bformat(const char* format, char* buffer, int& iteration, type argument, types ... arguments)
{
	while (*format)
	{
		if (*format != '{')
		{
			format++;
			iteration++;
			continue;
		}
		else
			format++;

		if (*format != '}')
		{
			format++;
			iteration++;
			continue;
		}
		else
		{
			buffer[iteration] = '%';
			buffer[++iteration] = cformat(argument);

			bformat(++format, buffer, ++iteration, arguments...);

			break;
		}
	}

	return buffer;
}

const char* format(const char* format)
{
	return format;
}

template<typename type>
const char* Format(const char* format, type argument)
{
	char* buffer = new char[strlen(format)];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	strcpy(buffer, format);
#pragma clang diagnostic pop

	for (auto iteration = 0; *format != '\0';)
	{
		if (*format != '{')
		{
			format++;
			iteration++;
			continue;
		}
		else
			format++;

		if (*format != '}')
		{
			format++;
			iteration++;
			continue;
		}
		else
		{
			buffer[iteration] = '%';
			buffer[++iteration] = cformat(argument);

			break;
		}
	}

	size_t size = snprintf(nullptr, 0, buffer, argument) + 1;
	auto returnable_buffer = new char[size];

	snprintf(returnable_buffer, size, buffer, argument);

	delete[] buffer;
	
	return returnable_buffer;
}

template<typename type, typename ... types>
const char* Format(const char* format, type argument, types ... arguments)
{
	char* buffer = new char[strlen(format)];

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	strcpy(buffer, format);
#pragma clang diagnostic pop

	for (auto iteration = 0; *format != '\0';)
	{
		if (*format != '{')
		{
			format++;
			iteration++;
			continue;
		}
		else
			format++;

		if (*format != '}')
		{
			format++;
			iteration++;
			continue;
		}
		else
		{
			buffer[iteration] = '%';
			buffer[++iteration] = cformat(argument);

			bformat(++format, buffer, ++iteration, arguments...);

			break;
		}
	}

	size_t size = snprintf(nullptr, 0, buffer, argument, arguments ...) + 1;
	auto returnable_buffer = new char[size];

	snprintf(returnable_buffer, size, buffer, argument, arguments...);

	delete[] buffer;

	return returnable_buffer;
}
