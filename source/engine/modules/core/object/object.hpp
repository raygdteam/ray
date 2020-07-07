#pragma once

struct RayObject;

using ObjectCreateFunc = RayObject * ();

struct RayObjectMeta
{
	char Name[128];
	ObjectCreateFunc CreateFunc;
};

struct RayObject
{
	
};
