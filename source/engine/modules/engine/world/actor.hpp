#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/components/transform.hpp>
#include <engine/engine/engine_def.hpp>

enum ActorTickStage : u8
{
	eEarlyTick = BIT(0),
	ePrePhysicsUpdate = BIT(1),
	ePostPhysicsUpdate = BIT(2),
	eLateUpdate = BIT(3)
};

enum MovementType : u8
{
	eStatic,
	eDynamic,
};

struct ActorTick
{
	bool Active;
	ActorTickStage Stage;
};

/**
 * Generic actor in scene.
 */
class RAY_ENGINE_API Actor : public RayObject
{
	friend class Level;
	friend class World;
	
	RAYOBJECT_BODY(Actor, RayObject);

	MovementType _movementType;
	Array<IComponent*> _components;
	
protected:
	ActorTick ATD;
	
	virtual void Awake() = 0;
	
	virtual void BeginPlay() = 0;
	virtual void Tick(f64 delta) = 0;
	virtual void OnDestroy() = 0;

	virtual void MoveTo(FVector2 pos)
	{
		if (_movementType == eStatic) return;
		GetTransform()->Position = pos;
	}
	
protected:
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
	MovementType GetMovementType()
	{ return _movementType; }

	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
};
