#include "core/core.hpp"
#include <Windows.h>
#include <stdio.h>
#include <core/memory/new_delete_override.hpp>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>

extern "C"
{
#include <kernel/kernel_thread.h>
}

#pragma comment(lib, "Dbghelp.lib")

/** Exports for video drivers to request the most powerful GPU available. Mainly for OGL/DX11 because Vulkan allows app to choose the GPU. */
extern "C" { _declspec(dllexport) u32 NvOptimusEnablement = 0x00000001; }
extern "C" { _declspec(dllexport) u32 AmdPowerXpressRequestHighPerformance = 0x00000001; }

int RayMain();

// Only allow single instance of the game running.
HANDLE gMutex;

void ReleaseMutex()
{
	if (gMutex)
	{
		ReleaseMutex(gMutex);
		gMutex = nullptr;
	}
}

/** Try to acquire mutex. Returns true if success(this is the only instance), false otherwise. */
bool MakeMutex()
{
	pcstr mutexName = "RAY_ENGINE";

	gMutex = CreateMutexA(0, true, mutexName);

	if (!gMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		/* This is second instance. Exit. */
		return false;
	}

	return true;
}

extern "C" RAY_DLLIMPORT void dummy();

u32 GuardedMain()
{
	/* Ensure kernel.dll is loaded */
	dummy();
	
	// TODO: switch to more appropriate solution: here, the compiler just fucking optimizes away the whole dx12 module.
#ifdef RAY_RELEASE
	//int foo(const int bar); foo(1);
#endif
	
	/* For Debug and Development allocate a console. */
	/* For some unknown reason works only in .exe module.*/
#if !defined(RAY_RELEASE)
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	DeleteMenu(GetSystemMenu(GetConsoleWindow(), false), SC_CLOSE, MF_BYCOMMAND);
#endif
	
	return RayMain();
}

int GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
{
	BOOL bMiniDumpSuccessful;
	HANDLE hDumpFile;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	hDumpFile = CreateFileA("ray_crash_dump.dmp", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	ExpParam.ThreadId = GetCurrentThreadId();
	ExpParam.ExceptionPointers = pExceptionPointers;
	ExpParam.ClientPointers = TRUE;

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithThreadInfo, &ExpParam, NULL, NULL);

	return EXCEPTION_EXECUTE_HANDLER;
}


int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//void WinMainCRTStartup()
{
	u32 returnCode = 0;

	/* If we are debugging then we don't need the SEH */
	if (IsDebuggerPresent())
	{
		return GuardedMain();
	}
	
	__try
	{
		returnCode = GuardedMain();
	}
	__except (GenerateDump(GetExceptionInformation()))
	{
		MessageBoxA(nullptr, "The engine crashed!", "Ray Engine - Error", MB_OK | MB_ICONERROR);
#if !defined(RAY_RELEASE)
		__debugbreak();
#endif
	}
	
	ExitProcess(returnCode);
}

