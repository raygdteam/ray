#include <core/core.hpp>
#include <core/object/object_db.hpp>
#include <core/lib/array.hpp>

#include <cstring>

namespace ray
{
static Array<RayObjectMeta*>* gObjects = nullptr;

ObjectDb::ObjectDb()
{
	gObjects = new Array<RayObjectMeta*>();
}

RayObjectMeta* ObjectDb::GetObject(pcstr name)
{
	return nullptr;
}

void ObjectDb::RegisterObject(RayObjectMeta* meta)
{
}
}
