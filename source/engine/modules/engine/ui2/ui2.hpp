#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>

class UiObject
{
	friend class UiRenderer;

protected:
	void Render();
	void Update();
public:
	const Array<UiObject&>& GetChildren();
	void AddChildren(UiObject*);
};

class UiWindow
{
	friend class UiRenderer;

protected:
	void Render();
	void Update();
public:
	const Array<UiObject&>& GetChildren();
	void AddChildren(UiObject*);
};

class UiRenderer
{
public:
	void RenderAll();
	const Array<UiWindow&>& GetWindows();
};
