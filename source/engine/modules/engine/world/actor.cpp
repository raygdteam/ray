#include "actor.hpp"
#include "core/extended_instuctions/sse/common.hpp"

static u64 gLastId = 0;

Actor::Actor()
{
	Transform* transform = new Transform();
	transform->Position = FVector<2>();
	transform->Position.x = 0.f;
	transform->Position.y = 0.f;
	transform->Scale = FVector2{ 1,1 };
	_components.PushBack(transform);

	Name = String("Actor #");
	Name.append_sprintf("%llu", gLastId++);
}

Actor::~Actor()
{
	for (u32 i = 0; i < _components.Size(); ++i)
	{
		delete _components.At(i);
	}
	_components.Clear();
}

Transform* Actor::GetTransform()
{
	return GetComponent<Transform>();
}

void Actor::Serialize(Archive& ar)
{
	ar.Write<u32>(Crc32((u8*)GetType()->Name, strlen(GetType()->Name)));
	ar.Write<u64>(_components.Size());

	// TODO: not like this!
	GetTransform()->Serialize(ar);
}

void Actor::Deserialize(Archive& ar)
{
	u64 numComponents = 0;
	ar.Read<u64>(numComponents);
	_components.Clear();
	
	// TODO: not like this!
	Transform* transform = new Transform();
	transform->Deserialize(ar);
	_components.PushBack(transform);
}

RAYOBJECT_DESCRIPTION_BEGIN(Actor)
	RAYOBJECT_DESCRIPTION_NAME("engine://world/Actor")
RAYOBJECT_DESCRIPTION_END(Actor)

