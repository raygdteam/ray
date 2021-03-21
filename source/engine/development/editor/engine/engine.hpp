#pragma once
#include <engine/engine/engine.hpp>
#include <engine/world/level.hpp>
#include <app_framework/base/platform_window.hpp>
#include <editor/renderer/renderer.hpp>

#include <editor/ui/editor_ui.hpp>

#include "editor_cmd.hpp"

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

	EditorUi _editorUi;

	std::queue<EditorCommand*> _pendingEditorCommands;

	void ProcessCommands();
public:
	void Initialize(IEngineLoop* engineLoop) override;
	void Tick() override;

	void ApplyMouseDragOnViewport(FVector2 drag);
	FVector2& GetCameraPos();
	void RunCommand(EditorCommand* command);

	float MouseDragSensitivity = 1.75f;

	Level* GetLevel()
	{
		return _level;
	}

	f32 GetDelta()
	{
		return _delta;
	}
};

extern EditorEngine* gEditorEngine;
