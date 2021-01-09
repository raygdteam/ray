#include <editor/engine/engine.hpp>
#include <editor/imgui/imgui.h>
#include <chrono>

#undef CreateWindow

void EditorEngine::Initialize(IEngineLoop* engineLoop)
{
	_window = IPlatformWindow::CreateInstance();

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
	
	bool data = false;
	float dat = 0.0f;
	int da = 0;
	static const char* a[] = { "straight x one two", "molten metal" };
	ImGui::Begin("Settings");

	ImGui::Checkbox("This game allows you to hear", &data);
	ImGui::SliderFloat("Watch", &dat, 0.f, 100.f);
	ImGui::SliderFloat("MP3 tricks", &dat, 0.f, 100.f);
	ImGui::SliderFloat("An electric motorcycle", &dat, 0.f, 100.f);

	ImGui::Separator();
	ImGui::Checkbox("The gears are drawing", &data);
	ImGui::Combo("Congratulations to the department of mathematics", &da, a, 2);
	
	
	ImGui::End();
	
	_renderer->EndScene();
	
	auto elapsed = std::chrono::high_resolution_clock::now() - __start;
	delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.f;
}
