#pragma once
#include <core/object/archive.hpp>

struct RayObject;

using ObjectCreateFunc = RayObject * ();

#define RAYOBJECT(Name)
#define RAYOBJECT_BODY(Type, Base) using Super = Base;
#define PROPERTY(Props)
#define METHOD(Props)

struct RayObjectMeta
{
	char Name[128];
	ObjectCreateFunc* CreateFunc;
};

struct RayObject
{
	RayObject() {}
	virtual ~RayObject() {}

	virtual void Serialize(Archive&) = 0;
	virtual void Deserialize(Archive&) = 0;
};
