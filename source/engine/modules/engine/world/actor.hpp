#pragma once
#include <core/object/object.hpp>
#include <engine/world/components/transform.hpp>
#include <engine/engine/engine_def.hpp>
#include "core/lib/array.hpp"

struct ActorTick
{
	bool Active : 1 = false;
	
};

/**
 * Generic actor in scene.
 */
class RAY_ENGINE_API Actor : public RayObject
{
	RAYOBJECT_BODY(Actor, RayObject);
	
	friend class Level;
	
	Array<IComponent*> _components;
protected:
	virtual void BeginPlay() = 0;
	virtual void Tick(f64 delta) = 0;
	virtual void OnDestroy() = 0;

	// I know. Terrible.
	// WARNING: SLOW!
	template <class Component>
	Component* GetComponent()
	{
		for (u32 i = 0; i < _components.Size(); ++i)
		{
			Component* target = dynamic_cast<Component*>(_components[i]);
			if (target != nullptr) return target;
		}

		return nullptr;
	}

public:
	Actor();
	virtual ~Actor();

	Transform* GetTransform();

	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
};
