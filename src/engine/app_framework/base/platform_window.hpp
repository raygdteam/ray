#pragma once

namespace ray::core
{

/** Interface for OS window. */
class IPlatformWindow
{
public:
	virtual ~IPlatformWindow() = default;

	/**
	 *  Create instance of @see IPlatformWindow according to the platform.
	 */
	IPlatformWindow* CreateInstance();

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
	virtual bool CreateWindow(char name[64]) = 0;

	/**
	 *  Sets the window visibility.
	 *  @param visible The visibility of the window.
	 */
	virtual void SetWindowVisibility(bool visible) = 0;

	/**
	 *  Returns the raw window handle.
	 *  HWND on Windows, for example.
	 */
	virtual void* GetWindowHandleRaw() = 0;

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
