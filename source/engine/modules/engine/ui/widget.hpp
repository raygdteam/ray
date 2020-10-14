#pragma once

#include <engine\ui\object.hpp>
#include <engine\ui\object_proxy.hpp>

class UIWidget
{
public:
	Array<UIObject*> _objects;
	Array<UIObjectProxy*> _proxies;

	void AddObject(UIObject&);
	void RemoveObject(UIObject&); // ?
};