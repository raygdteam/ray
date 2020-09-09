#pragma once

#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

// Internal use only.
struct WorldLevelData;

class World final
{
	/* Not an array since we only support one level at a time. */
	WorldLevelData* _levelData = nullptr;

	/* Functionality to support level reloads on beginning of next frame. */
	bool _shouldLoadLevel = false;
	UninitializedResourceRef _levelRef;

	/* Run tick at specified stage. */
	void TickActors(ActorTickStage stage, f64 delta) const;

	/* Run physics update. */
	void PhysicsUpdate();

	/* Render the level. */
	void Render();
public:
	void Initialize();
	void ScheduleLevelReload(UninitializedResourceRef level);
	void Tick(f64 delta);
};

