#pragma once

#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <core/threading/condition_variable.hpp>

#include <engine/world/actor.hpp>
#include <engine/world/actors/camera_actor.hpp>
#include <renderer_core/renderer.hpp>

using namespace ray::renderer_core_api;

// Internal use only.
struct WorldLevelData
{
	Level* Level;
	/* for level streaming
	 * FVector3 Position;
	 */
};

class World final
{
	/* Delta is calculated by Engine and passed in Tick. */
	f64 _delta;
	
	/* Not an array since we only support one level at a time. */
	WorldLevelData* _levelData = nullptr;

	CameraActor* _primaryCameraActor = nullptr;

	IRenderer* _renderer = nullptr;

	/* Functionality to support level reloads on beginning of next frame. */
	// bool _shouldLoadLevel = false;
	// UninitializedResourceRef _levelRef;

	/* Run tick at specified stage. */
	void TickActors(ActorTickStage stage, f64 delta) const;

	/* Run physics update. */
	void PhysicsUpdate();

	/* Render the level. */
	void Render();

	void RendererInitialize(ray::core::IPlatformWindow* window);

	void WorldTickThread();
	void RenderingThread();
public:
	void Initialize(ray::core::IPlatformWindow* window);
	// void ScheduleLevelReload(UninitializedResourceRef level);
	void Tick(f64 delta);

	void SetPrimaryCamera(CameraActor* camera);

	ConditionVariable ReadyToTick;
	ConditionVariable WorldTickFinished;
	ConditionVariable RenderingFinished;
	ConditionVariable TickFinished;
};

