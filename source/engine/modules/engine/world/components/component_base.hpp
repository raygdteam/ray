#pragma once
#include <core/object/object.hpp>

class Actor;
class Level;
class World;

class IComponent : public RayObject
{
	RAYOBJECT_BODY(IComponent, RayObject);
	friend class Actor;
	friend class Level;
	
protected:
	Actor* _parent = nullptr;
	World* _world = nullptr;
	Level* _level = nullptr;

	void Setup(World* world, Level* level)
	{
		_world = world;
		_level = level;
	}
public:
	virtual ~IComponent() = default;
	
	virtual void Init() = 0;
	virtual void Tick() = 0;
	virtual void OnDestroy() = 0;

	virtual void Serialize(Archive&) override = 0;
	virtual void Deserialize(Archive&) override = 0;
};
