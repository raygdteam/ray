#include <core/core.hpp>
#include <core/object/object_db.hpp>
#include <core/lib/array.hpp>

#include <cstring>s

static ray::Array<RayObjectMeta*>* gObjects = nullptr;

ObjectDb::ObjectDb()
{
	gObjects = new ray::Array<RayObjectMeta*>();
}

RayObjectMeta* ObjectDb::GetObject(pcstr name)
{
	if (!gObjects->IsEmpty())
	{
		for (RayObjectMeta* object : *gObjects)
		{
			if (strcmp(object->Name, name) == 0)
				return object;
		}
	}

	return nullptr;
}

void ObjectDb::RegisterObject(RayObjectMeta* meta)
{
	gObjects->Push(meta);
}
