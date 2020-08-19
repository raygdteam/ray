#include "actor.hpp"

Actor::Actor()
{
	Transform* transform = new Transform();
	transform->Position = FVector<2>();
	transform->Position.x = 0.f;
	transform->Position.y = 0.f;
	_components.PushBack(transform);
}

Actor::~Actor()
{
	for (u32 i = 0; i < _components.Size(); ++i)
	{
		delete _components[i];
	}
}

Transform* Actor::GetTransform()
{
	return GetComponent<Transform>();
}

void Actor::Serialize(Archive& ar)
{
	ar.Write<u64>(_components.Size());

	// TODO: not like this!
	GetTransform()->Serialize(ar);
}

void Actor::Deserialize(Archive& ar)
{
	u64 numComponents = 0;
	ar.Read<u64>(numComponents);
	_components.clear();
	
	// TODO: not like this!
	Transform* transform = new Transform();
	transform->Deserialize(ar);
	_components.PushBack(transform);
}
