#pragma once
#include <core/core.hpp>

using UIObjectId = u64;
constexpr UIObjectId NullUIObject = (UIObjectId)-1;

class IUIObject
{
	UIObjectId _id = NullUIObject;
public:
	UIObjectId GetId()
	{ return _id; }
protected:
	/* state... */
};
