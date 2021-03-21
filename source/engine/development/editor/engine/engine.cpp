#include <editor/engine/engine.hpp>
#include <core/log/log.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <engine/ui2/ext/imgui.h>
#include <engine/ui2/ext/imgui_internal.h>

#include <chrono>
#include <engine/world/actors/static_quad_actor.hpp>
#include <engine/world/world.hpp>
#include <editor_core/caches/component_cache.hpp>
#include <editor_core/caches/actor_cache.hpp>
#include <renderer_core/resources/buffer_manager.hpp>
#include <renderer_core/command_context.hpp>


#include "editor/windows/debug_window.hpp"
#include <editor/windows/level_viewport.hpp>

#include "editor/windows/level_outline.hpp"
#include "editor/windows/log_window.hpp"
#include "engine/ui2/ui2.hpp"
#include "renderer_core/renderer_2d.hpp"

#undef CreateWindow

IRenderer* gRenderer;
EditorEngine* gEditorEngine;
Logger* gEditorLogger;


void EditorEngine::ProcessCommands()
{
	while (!_pendingEditorCommands.empty())
	{
		EditorCommand* cmd = _pendingEditorCommands.back();

		if (cmd->Type == eLoadLevel)
		{
			EditorCommand_LoadLevel* loadLevelCmd = reinterpret_cast<EditorCommand_LoadLevel*>(cmd);
			gEditorLogger->Log("BEGIN Loading Level !!!");
			_world->LoadLevel(loadLevelCmd->Path);
			gEditorLogger->Log(" END  Loading Level !!!");
			_level = _world->_levelData->Level;

			_editorUi.CmdLevelLoaded();
		}
		else if (cmd->Type == eCloseLevel)
		{
			_world->UnloadLevel();
			_level = nullptr;
			_editorUi.CmdCloseLevel();
		}
		
		_pendingEditorCommands.pop();
		delete cmd;
	}
}

void EditorEngine::Initialize(IEngineLoop* engineLoop)
{
	gEditorEngine = this;
	gEditorLogger = new Logger("EditorEngine");
	
	_window = IPlatformWindow::CreateInstance();

	_window->Initialize();
	_window->CreateWindow("RAY_EDITOR");

	gComponentCache = new ComponentCache;
	gComponentCache->Rebuild();

	gActorCache = new ActorCache();
	gActorCache->Rebuild();
	
	gRenderer = new IRenderer();
	gRenderer->Initialize(_window);

	_world = new World();
	_world->Initialize(nullptr);

	_world->RendererInitialize(nullptr);

	_editorUi.Initialize(_window);

	_window->SetWindowVisibility(true);
}

void EditorEngine::Tick()
{
	auto __start = std::chrono::high_resolution_clock::now();

	_window->Update();
	if (_window->ShouldClose())
	{
		RequestEngineExit(true);
		return;
	}

	ProcessCommands();

	_editorUi.Tick();
		
	GraphicsContext& ctx = GraphicsContext::Begin();

	if (_level != nullptr)
	{
		gRenderer->Begin(gSceneColorBuffer, ctx);
		{
			_world->RenderEditor(ctx);
		}
		gRenderer->End(gSceneColorBuffer, ctx);
	}
	
	gRenderer->Begin(gEditorColorBuffer, ctx);
	{
		_editorUi.Render(ctx);
	}
	gRenderer->End(gEditorColorBuffer, ctx);
	gRenderer->Present(gEditorColorBuffer, ctx);
	
	auto elapsed = std::chrono::high_resolution_clock::now() - __start;
	_delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.f;
}

void EditorEngine::ApplyMouseDragOnViewport(FVector2 drag)
{	
	_world->_primaryCameraActor->GetTransform()->Position.x -= drag.x * _delta * MouseDragSensitivity;
	_world->_primaryCameraActor->GetTransform()->Position.y += drag.y * _delta * MouseDragSensitivity;

	_world->_primaryCameraActor->GetCameraComponent()->UpdateMVP();
}

void EditorEngine::ApplyMouseZoomOnViewport(f32 dz)
{
	((CameraActor*)(_world->_primaryCameraActor))->GetCameraComponent()->Zoom += ((dz/10.f) * _delta);

	_world->_primaryCameraActor->GetCameraComponent()->UpdateMVP();
}

FVector2& EditorEngine::GetCameraPos()
{
	return _world->_primaryCameraActor->GetTransform()->Position;
}

float* EditorEngine::GetCameraZoom()
{
	return &((CameraActor*)(_world->_primaryCameraActor))->GetCameraComponent()->Zoom;
}

void EditorEngine::ResizeCameraViewport(FVector2 size)
{
	((CameraActor*)(_world->_primaryCameraActor))->GetCameraComponent()->ViewportSize = size;

	_world->_primaryCameraActor->GetCameraComponent()->UpdateMVP();
}

void EditorEngine::RunCommand(EditorCommand* command)
{
	_pendingEditorCommands.push(command);
}
