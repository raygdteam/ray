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
};

class UiRootObject
{
public:
	void RenderAll();
	void AddObject(UiObject* root, UiObject*);
	const Array<UiObject&>& GetWindows();
};
