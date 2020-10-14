#pragma once

#include <engine\ui\object.hpp>
#include <engine\ui\object_proxy.hpp>

class UiWidget
{
public:
	Array<ui::object*> _objects;
	Array<ui::object_proxy*> _proxies;

	void AddObject(ui::object&);
	void RemoveObject(ui::object&); // ?
};