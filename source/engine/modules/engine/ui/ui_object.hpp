#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>

class IUIObject
{
	Array<IUIObject*> _subobjects;
};
