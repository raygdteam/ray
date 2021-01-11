#include <core/core.hpp>
#include <core/object/object.hpp>
#include <core/lib/array.hpp>

#include <cstring>
#include "core/extended_instuctions/sse/common.hpp"


static Array<Type*>* gObjects = nullptr;

ObjectDb::ObjectDb()
{
	if (gObjects == nullptr)
	{
		gObjects = new Array<Type*>();
	}
}

Type* ObjectDb::GetTypeByName(pcstr name)
{
	for (Type* type : *gObjects)
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
	for (Type* type : *gObjects)
	{
		if (type->NameCrc32 == crc)
		{
			return type;
		}
	}

	return nullptr;
}

void ObjectDb::__Internal_RegisterObjectStatic(Type* type)
{
	if (gObjects == nullptr)
	{
		gObjects = new Array<Type*>();
	}

	if (type->NameCrc32 == 0)
		type->NameCrc32 = ray::core::sse::Crc32((u8*)type->Name, strlen(type->Name));
	
	gObjects->PushBack(type);
}

