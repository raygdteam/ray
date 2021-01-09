#include <core/core.hpp>
#include <core/log/log.hpp>
#include <core/threading/critical_section.hpp>
#include <core/debug/assert.hpp>

#include <cstdlib>
#include <memory>
#include <cstdarg>


// TODO: remove dependecy
#include <Windows.h>

static u64 gTimestamp = 0;
static CriticalSection* gLoggerMutex = nullptr;

Logger::Logger(pcstr name)
{
	_name = String(name);

	if (gTimestamp == 0) gTimestamp = GetTickCount64();
	if (gLoggerMutex == nullptr) gLoggerMutex = new CriticalSection();
}

Logger::~Logger()
{
}

void Logger::Log(pcstr msg, ...)
{
	/* Get timestamp. We need to call it as early as possible.*/
	char time[64] = {};
	sprintf(time, "%f", f32(GetTickCount64() - gTimestamp) / 1000.f);

	/* Format user string. */
	va_list mark = {};
	char buf[2048];
	va_start(mark, msg);
	_vsnprintf_s(buf, sizeof(buf) - 1, msg, mark);
	ULONG bufSize = sizeof(buf) - 1;
	buf[bufSize] = 0;
	va_end(mark);

	char str[4096] = {};
	sprintf(str, "[%s] {%s}: %s\n", time, _name.AsRawStr(), buf);

	gLoggerMutex->Enter();

	OutputDebugStringA(str);

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
	printf("%s", str);
#endif
	gLoggerMutex->Leave();
}