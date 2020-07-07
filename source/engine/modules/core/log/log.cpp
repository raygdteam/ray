#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <Windows.h>
#include <core/core.hpp>
#include "log.hpp"
#include <cstdio>

using pstr = char*;
using pcstr = const char*;

static u64 gTimestamp = 0;


Logger::Logger(pcstr name)
{
	_name = (pstr)malloc(17 * sizeof(char));
	_name[16] = '\0';

	// Log name can be max. 16 chars
	if (strlen(name) > 16) assert(false);

	strcpy(_name, name);

	if (gTimestamp == 0) gTimestamp = GetTickCount64();
}

Logger::~Logger()
{
	free(_name);
}

void Logger::Log(pcstr msg)
{
	char str[512] = {};
	strcat(str, "{");
	strcat(str, _name);

	for (size_t i = strlen(_name); i <= 17; i++)
		strcat(str, " ");

	char time[64] = {};
	_itoa(int(GetTickCount64() - gTimestamp), time, 10);

	strcat(str, time);

	for (size_t i = strlen(time); i <= 10; i++)
		strcat(str, " ");

	strcat(str, "}   ");
	strcat(str, msg);
	strcat(str, "\n");

	OutputDebugStringA(str);

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	printf(str);
#endif
}
