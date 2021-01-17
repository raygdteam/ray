#include <core/core.hpp>
#include <core/object/object.hpp>
#include <core/lib/array.hpp>

#include <cstring>
#include "core/extended_instuctions/sse/common.hpp"

static Type* gStaticallyLoadedObjects[32] = {};

ObjectDb::ObjectDb()
{
	for (Type* mod : gStaticallyLoadedObjects)
	{
		if (mod == nullptr)
			continue;

		_objects.PushBack(mod);
	}

	memset(&gStaticallyLoadedObjects, 0, sizeof(Type*) * 32);
}

Type* ObjectDb::GetTypeByName(pcstr name)
{
	for (Type* type : _objects)
	{
		if (strcmp(type->Name, name) == 0)
		{
			return type;
		}
	}
	
	return nullptr;
}

Type* ObjectDb::GetTypeByCrc(u32 crc)
{
	for (Type* type : _objects)
	{
		if (type->NameCrc32 == crc)
		{
			return type;
		}
	}

	return nullptr;
}

Array<Type*>& ObjectDb::GetAllTypes()
{
	return _objects;
}

void ObjectDb::__Internal_RegisterObjectStatic(Type* type)
{
	if (type->NameCrc32 == 0)
		type->NameCrc32 = Crc32((u8*)type->Name, strlen(type->Name));
	
	for (u8 i = 0; i < 32; ++i)
	{
		if (gStaticallyLoadedObjects[i] == nullptr)
		{
			gStaticallyLoadedObjects[i] = type;
			return;
		}
	}
}

