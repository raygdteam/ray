#pragma once
#include <core/core.hpp>
#include <engine/ui2/ui2.hpp>

class EditorUi
{
	UiRootObject* _rootObject = nullptr;
public:
	EditorUi();
	void Initialize(IPlatformWindow* window);
	void Tick();
	void Render(GraphicsContext& ctx);
};
