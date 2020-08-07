#pragma once

struct RayObject;

using ObjectCreateFunc = RayObject * ();

#define RAYOBJECT()
#define RAYOBJECT_BODY()
#define PROPERTY()
#define METHOD()

struct RayObjectMeta
{
	char Name[128];
	ObjectCreateFunc* CreateFunc;
};

struct RayObject
{
	
};
