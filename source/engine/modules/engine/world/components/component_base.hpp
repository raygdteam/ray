#pragma once
#include <core/object/object.hpp>

class IComponent : RayObject
{
	// RAYOBJECT_BODY(IComponent, RayObject);
	
public:
	virtual ~IComponent() = default;
	
	virtual void Init() = 0;
	virtual void Tick() = 0;
	virtual void OnDestroy() = 0;

	virtual void Serialize(Archive&) override = 0;
	virtual void Deserialize(Archive&) override = 0;
};
