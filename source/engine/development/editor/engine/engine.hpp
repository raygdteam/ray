#pragma once
#include <engine/engine/engine.hpp>
#include <engine/world/level.hpp>
#include <app_framework/base/platform_window.hpp>
#include <editor/renderer/renderer.hpp>

#ifdef RAY_BUILD_EDITOR
#define EDITOR_API RAY_DLLEXPORTS
#else
#define EDITOR_API RAY_DLLIMPORT
#endif

class EDITOR_API EditorEngine : public IEngine
{
	IPlatformWindow* _window = nullptr;
	float _delta = .0f;
	//IVkRenderer* _renderer = nullptr;
	World* _world = nullptr;
	Level* _level = nullptr;
public:
	void Initialize(IEngineLoop* engineLoop) override;
	void Tick() override;

	void ApplyMouseDragOnViewport(FVector2 drag);
	FVector2& GetCameraPos();

	float MouseDragSensitivity = 1.75f;
};

extern EditorEngine* gEditorEngine;
