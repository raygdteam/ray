#pragma once
#include "event.hpp"
#include "window_event.h"
#include "../Core/core.h"
#include <Windows.h>

class IPlatform;

//static class. it has access to IPlatform
class Platform
{
public:
#ifdef RAY_PLATFORM_WIN
	static void Init(HINSTANCE hInstance, LPSTR lpCmdLine, s32 nCmdShow);
	static HWND GetHWND();
#elif RAY_PLATFORM_LINUX
	static void Init(char** argc, int argv);
#endif //RAY_PLATFORM_WIN
	static void Destroy();

	static ray_string GetPlatformName();
	static u16 GetWidth();
	static u16 GetHeight();

	static void OnEvent();
	static bool WindowIsOpen();

	static void SetCallback(EventCallback callback);

	static bool HasFeature(ray::core::hardware::Feature feature);

	static bool CanTick();
};


