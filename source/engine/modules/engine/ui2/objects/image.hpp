#pragma once
#include <engine/ui2/ui2.hpp>

/**
 * Temporarily only for gSceneColorBuffer
 */
class RAY_ENGINE_API UiImage : public UiObject
{
public:
	UiImage(UiWindow* window);
	void Tick() override;
};

