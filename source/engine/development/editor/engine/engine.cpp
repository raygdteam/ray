#include <editor/engine/engine.hpp>
#include <editor/imgui/imgui.h>
#include <chrono>

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
	static f64 delta = 1;
	auto __start = std::chrono::high_resolution_clock::now();

	_window->Update();
	if (_window->ShouldClose())
	{
		RequestEngineExit(true);
		return;
	}

	_renderer->BeginScene();
	if (ImGui::BeginMainMenuBar())
	{
		
		ImGui::Text("FPS: %i", u32(1000.f / delta));
		ImGui::Separator();
		
		ImGui::EndMainMenuBar();
	}
	_renderer->EndScene();
	
	auto elapsed = std::chrono::high_resolution_clock::now() - __start;
	delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.f;
}
