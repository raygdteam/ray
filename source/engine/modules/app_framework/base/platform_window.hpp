#pragma once
#include <core/lib/delegate.hpp>
#include "app_framework/app_framework.hpp"


#undef CreateWindow

namespace ray::core
{

/** Interface for OS window. */
class IPlatformWindow
{
public:
	virtual ~IPlatformWindow() = default;

	/**
	 *  Create instance of IPlatformWindow according to the platform.
	 */
	static RAY_APPFRAMEWORK_API IPlatformWindow* CreateInstance();

	/** 
	 *  Called to initialize required systems. 
	 *  This shouldn't create a window yet.
	 *  TODO: add a parameter for cmd line.
	 */
	virtual void Initialize() = 0;

	/**
	 *  Called to create a window. This should not show it yet.
	 *  @param name The title of window.
	 *  @returns True if success, false otherwise.
	 */
	virtual bool CreateWindow(const char* name) = 0;

	/**
	 *  Sets the window visibility.
	 *  @param visible The visibility of the window.
	 */
	virtual void SetWindowVisibility(bool visible) = 0;

	/**
	 *  Runs the window update.
	 */
	virtual void Update() = 0;

	/**
	 *  Should the window close?
	 */
	virtual bool ShouldClose() = 0;

	/**
	 *  Returns the raw window handle.
	 *  HWND on Windows, for example.
	 */
	virtual void* GetWindowHandleRaw() = 0;

	virtual void RegisterEventCallback(Function<void(void*, u32, u64, s64)> callback) = 0;

	/**
	*	Returns the window width
	*/
	virtual u16 GetWidth() = 0;

	/**
	*	Returns the window height
	*/
	virtual u16 GetHeight() = 0;

	/**
	 *  Destroy the window.
	 */
	virtual void Destroy() = 0;

	/**
	 *  Shutdown the IPlatformWindow.
	 */
	virtual void Shutdown() = 0;
};

}
