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

	strcpy_s(_name, strlen(name), name);

	if (gTimestamp == 0) gTimestamp = GetTickCount64();
}

Logger::~Logger()
{
	free(_name);
}

void Logger::Log(pcstr msg)
{
	char str[512] = {};
	strcat_s(str, "{");
	strcat_s(str, _name);

	for (size_t i = strlen(_name); i <= 17; i++)
		strcat_s(str, " ");

	char time[64] = {};
	_itoa_s(int(GetTickCount64() - gTimestamp), time, 10);

	strcat_s(str, time);

	for (size_t i = strlen(time); i <= 10; i++)
		strcat_s(str, " ");

	strcat_s(str, "}   ");
	strcat_s(str, msg);
	strcat_s(str, "\n");

	OutputDebugStringA(str);

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	printf(str);
#endif
}
