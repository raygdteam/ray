#include <core/debug/debug.hpp>
#include <core/log/log.hpp>

#include <csignal>

static Logger* gLog = nullptr;

namespace ray
{
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

bool Debug::Initialize()
{
	gLog = new Logger("debug");

	signal(SIGINT, Debug::handleSignal); // interrupt
	signal(SIGILL, Debug::handleSignal); // illegal instruction
	signal(SIGFPE, Debug::handleSignal); // FPU error
	signal(SIGSEGV, Debug::handleSignal); // Segmentation fault
}
}
