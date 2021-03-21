#pragma once
#include <core/core.hpp>
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <core/threading/condition_variable.hpp>
#include <core/threading/thread_pool.hpp>

#include <engine/world/actor.hpp>
#include <engine/world/actors/camera_actor.hpp>
#include <renderer_core/renderer.hpp>

#include "renderer_core/resources/gpu_texture.hpp"


// Internal use only.
struct WorldLevelData
{
	Level* Level;
	/* for level streaming
	 * FVector3 Position;
	 */
};

struct MaterialInstance
{
	u64 Id;
	String Name;
	GpuTexture* Texture;
	TextureView TextureView;
	// ...
};

struct MaterialCompileProperties
{
	String Name;
	String Texture;
};

class RAY_RENDERERCORE_API UploadBuffer;

class RAY_ENGINE_API World final
{
	friend class EditorEngine;
	
	/* Delta is calculated by Engine and passed in Tick. */
	f64 _delta = 0.f;
	
	/* Not an array since we only support one level at a time. */
	WorldLevelData* _levelData = nullptr;

	CameraActor* _primaryCameraActor = nullptr;

	IRenderer* _renderer = nullptr;

	ThreadPool& _pool;

	Array<MaterialInstance> _materialInstances;
	
	/* Functionality to support level reloads on beginning of next frame. */
	// bool _shouldLoadLevel = false;
	// UninitializedResourceRef _levelRef;

	void TickActors(f64 delta) const;

	/* Run physics update. */
	void PhysicsUpdate();

	/* Render the level. */
	void Render();
	void RenderEditor(GraphicsContext& ctx);

	void RendererInitialize(IPlatformWindow* window);

	void WorldTickThread();
	void RenderingThread();

	void LoadLevelInternal(String& name);
public:
	World();
	~World();

	void Initialize(IPlatformWindow* window);
	void Tick(f64 delta);

	void Destroy() noexcept;

	void SetPrimaryCamera(CameraActor* camera);

	u64 CompileMaterial(MaterialCompileProperties& props);

	MaterialInstance& GetMaterialInstance(u64 id);
	u64 GetMaterialIdForName(String& name);

	void LoadLevel(String& path);
	
	ConditionVariable ReadyToTick;
	ConditionVariable WorldTickFinished;
	ConditionVariable RenderingFinished;
	ConditionVariable TickFinished;
};

