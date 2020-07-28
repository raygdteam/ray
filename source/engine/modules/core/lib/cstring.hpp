#pragma once

#include <malloc.h>

size_t cstrlen(const char* string)
{
	for (size_t length = 0; ; ++string, ++length)
		if (!*string)
			return length;
}

bool cstrcmp(const char* str_f, const char* str_s)
{
	int i = 0;

	for (int i = 0; str_f[i] == str_s[i] && i < cstrlen(str_f); i++);

	return (i == cstrlen(str_f)) ? true : false;
}

const char* subcstr(const char* src, int m, int n)
{
	int len = n - m;

	char* dest = (char*)malloc(sizeof(char) * (len + 1));

	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
		*dest = *(src + i);
		dest++;
	}

	*dest = '\0';

	return dest - len;
}