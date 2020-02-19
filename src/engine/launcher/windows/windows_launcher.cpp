#include "core/core.hpp"
#include "core/memory/memory.hpp"
#include <ray/os/include.hpp>
#include <shellapi.h>

/** Exports for video drivers to request the most powerful GPU available. Mainly for OGL/DX11 because Vulkan allows app to choose the GPU. */
extern "C" { _declspec(dllexport) u32 NvOptimusEnablement = 0x00000001; }
extern "C" { _declspec(dllexport) u32 AmdPowerXpressRequestHighPerformance = 0x00000001; }

int RayMain(pcwstr, s32);

// Only allow single instance of the game running.
HANDLE gMutex;

void ReleaseMutex()
{
	if(gMutex)
	{
		ReleaseMutex(gMutex);
		gMutex = nullptr;
	}
}

/** Try to acquire mutex. Returns true if success(this is the only instance), false otherwise. */
bool MakeMutex()
{
	pcstr mutexName = "RAY_ENGINE";

	gMutex = CreateMutex(0, true, mutexName);

	if(!gMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// This is second instance. Exit.
		return false;
	}

	return true;
}

u32 GuardedMain()
{
	// Wrap command line here. Basically cleanup.
	pcwstr commandLine = GetCommandLineW();
	s32 count;
	CommandLineToArgvW(commandLine, &count);

	/* For Debug and Development allocate a console. */
	/* For some unknown reason works only in .exe module.*/
	AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	RayMain(commandLine, count);
	return 0;
}


int __stdcall wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//void WinMainCRTStartup()
{
	u32 returnCode = 0;
	returnCode = GuardedMain();
	return returnCode;
	//ExitProcess(returnCode);
}

MEMORY_OVERRIDE;
