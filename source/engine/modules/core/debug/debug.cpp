#include <core/debug/debug.hpp>
#include <core/log/log.hpp>

#include <csignal>
#include <windows.h>
#include <cstdio>

static ray::core::log::logger* gLog = nullptr;

void Debug::handleSignal(int sc)
{
	gLog->Log("-------------- CRT SIGNAL --------------");
	switch (sc)
	{
	case SIGINT:
		gLog->Log("Debugger interrupt?");
		break;
	case SIGILL:
		gLog->Log("Illigel instruction");
		break;
	case SIGFPE:
		gLog->Log("FPU Error");
		break;
	case SIGSEGV:
		gLog->Log("Segmentation fault");
		break;
	default:
		gLog->Log("Unknown signal");
		break;
	}
	gLog->Log("----------------------------------------");
}

Debug::Debug()
{
	gLog = new ray::core::log::logger("debug");
	signal(SIGINT, Debug::handleSignal); // interrupt
	signal(SIGILL, Debug::handleSignal); // illegal instruction
	signal(SIGFPE, Debug::handleSignal); // FPU error
	signal(SIGSEGV, Debug::handleSignal); // Segmentation fault
}

void Debug::__Internal_HandleAssertionFailure(pcstr expression, pcstr file, u16 line, pcstr message)
{
	gLog->Log("---------- ASSERTION FAILURE ----------");
	gLog->Log("  Expression:       {}", expression);
	gLog->Log("  Message:          {}", message);
	gLog->Log("  File:             {}", file);
	gLog->Log("  Line:             {}", line);
	gLog->Log("---------------------------------------");

	if (IsDebuggerPresent())
	{
		__debugbreak();
	}

	char msg[4096] = {};
	sprintf(msg, "---------- ASSERTION FAILURE ----------\n  Expression:       %s\n  Message:          %s\n  File:             %s\n  Line:             %i\n---------------------------------------\nWould you like to terminate engine?",
		expression, message, file, line);

	u32 rc = MessageBoxA(nullptr, msg, "RAY_ENGINE - Assertion failure", MB_ICONERROR | MB_YESNO);
	if (rc == IDYES)
		ExitProcess(-1);
}
