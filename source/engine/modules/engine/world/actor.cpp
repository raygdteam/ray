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
