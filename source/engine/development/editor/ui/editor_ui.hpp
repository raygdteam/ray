#pragma once
#include <core/core.hpp>
#include <engine/ui2/ui2.hpp>


#include <editor/windows/debug_window.hpp>
#include <editor/windows/level_outline.hpp>
#include <editor/windows/level_viewport.hpp>
#include <editor/windows/log_window.hpp>
#include <editor/windows/level_chooser.hpp>
#include <editor/windows/actor_properties.hpp>

class EditorUi
{
	enum EditorUiState
	{
		eInvalid = 0,
		eChooseLevel,
		eEditingLevel
	};

	EditorUiState _state;
	
	UiRootObject* _rootObject = nullptr;

	LevelChooser* _levelChooser = nullptr;

	EdDebugWindow* _debugWindow = nullptr;
	EdLevelOutline* _levelOutline = nullptr;
	EdLevelViewport* _levelViewport = nullptr;
	EdLogWindow* _logWindow = nullptr;
	EdActorProperties* _actorProperties = nullptr;

public:
	EditorUi();
	void Initialize(IPlatformWindow* window);
	void Tick();
	void Render(GraphicsContext& ctx);

	void CmdLevelLoaded();
	void CmdCloseLevel();
};
