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
	_name = static_cast<pstr>(malloc(17 * sizeof(char)));
	memset(_name, 0, 17 * sizeof(char));
	_name[16] = '\0';

	// Log name can be max. 16 chars
	if (strlen(name) > 16) assert(false);

	memcpy(_name, name, strlen(name) * sizeof(char));

	if (gTimestamp == 0) gTimestamp = GetTickCount64();
}

Logger::~Logger()
{
	free(_name);
}

void Logger::Log(pcstr msg, ...)
{
	/* 1. Get timestamp. We need to call it as early as possible.*/
	char time[64] = {};
	_itoa_s(int(GetTickCount64() - gTimestamp), time, 10);

	/* 2. Format user string. */
	va_list mark;
	char buf[2048];
	va_start(mark, msg);
	int sz = _vsnprintf_s(buf, sizeof(buf) - 1, msg, mark);
	ULONG bufSize = sizeof(buf) - 1;
	buf[bufSize] = 0;
	va_end(mark);

	
	/* 3. Format user string. */
	char str[512] = {};
	strcat_s(str, "{");
	strcat_s(str, _name);

	for (size_t i = strlen(_name); i <= 17; i++)
		strcat_s(str, " ");

	strcat_s(str, time);

	for (size_t i = strlen(time); i <= 10; i++)
		strcat_s(str, " ");

	strcat_s(str, "}   ");
	strcat_s(str, buf);
	strcat_s(str, "\n");

	OutputDebugStringA(str);

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	printf(str);
#endif
}
