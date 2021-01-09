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
static f64 gPCFrequency = 0.0;
static CriticalSection* gLoggerMutex = nullptr;

f64 _GetLoggerTimestamp()
{
	LARGE_INTEGER counter = {};
	QueryPerformanceCounter(&counter);
	return f64(counter.QuadPart - gTimestamp) / gPCFrequency;
}

Logger::Logger(pcstr name)
{
	_name = String(name);

	LARGE_INTEGER counter = {};
	QueryPerformanceFrequency(&counter);
	gPCFrequency = f64(counter.QuadPart) / 1000.0;

	if (gTimestamp == 0)
	{
		QueryPerformanceCounter(&counter);
		gTimestamp = counter.QuadPart;
	}
	
	if (gLoggerMutex == nullptr) gLoggerMutex = new CriticalSection();
}

Logger::~Logger()
{
}

void Logger::Log(pcstr msg, ...)
{
	/* Get timestamp. We need to call it as early as possible.*/
	char time[64] = {};
	sprintf(time, "%f", _GetLoggerTimestamp() / 1000.0);

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