#include <editor/engine/engine.hpp>

#undef CreateWindow

void EditorEngine::Initialize(IEngineLoop* engineLoop)
{
	_window = ray::core::IPlatformWindow::CreateInstance();

	_window->Initialize();
	_window->CreateWindow("RAY_EDITOR");

	_renderer = new IVkRenderer();
	_renderer->Initialize(_window);

	_window->SetWindowVisibility(true);
}

void EditorEngine::Tick()
{
	_window->Update();
	if (_window->ShouldClose())
	{
		RequestEngineExit(true);
		return;
	}

	_renderer->BeginScene();
	_renderer->EndScene();
}
