#include <core/core.hpp>
#include <core/object/object.hpp>
#include <core/lib/array.hpp>

#include <cstring>


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

void ObjectDb::__Internal_RegisterObjectStatic(Type* type)
{
	if (gObjects == nullptr)
	{
		gObjects = new Array<Type*>();
	}
	
	gObjects->PushBack(type);
}

