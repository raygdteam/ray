#pragma once
#include <engine/world/actor.hpp>

class RAY_ENGINE_API StaticQuadActor : public Actor
{
	RAYOBJECT_BODY(StaticQuadActor, Actor);
public:
	StaticQuadActor();
	~StaticQuadActor();
protected:
	void Awake() override;
	void BeginPlay() override;
	void Tick(f64 delta) override;
	void OnDestroy() override;
};
