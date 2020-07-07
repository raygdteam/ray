#include "object_db.hpp"

#include <cstring>

struct ObjectLinkedList
{
	RayObjectMeta* Meta;
	ObjectLinkedList* Next;
};

static ObjectLinkedList* _objects = nullptr;

ObjectDb::ObjectDb()
{
	
}

RayObjectMeta* ObjectDb::GetObject(pcstr name)
{
	if (_objects != nullptr)
	{
		ObjectLinkedList* current = _objects;

		while (current)
		{
			if (strcmp(current->Meta->Name, name) == 0)
				return current->Meta;
			
			current = current->Next;
		}
	}

	return nullptr;
}

void ObjectDb::RegisterObject(RayObjectMeta* meta)
{
	if (_objects == nullptr)
	{
		_objects = new ObjectLinkedList { meta, nullptr };
		return;
	}

	ObjectLinkedList* current = _objects;

	while (current)
	{
		if (current->Next == nullptr)
		{
			current->Next = new ObjectLinkedList{ meta, nullptr };
			return;
		}
		
		current = current->Next;
	}
}
