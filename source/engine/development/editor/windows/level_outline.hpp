#pragma once
#include <engine/ui2/ui2.hpp>
#include <engine/world/actor.hpp>

extern Actor* gSelectedActor;

class EdLevelOutline : public UiWindow
{
public:
	EdLevelOutline();
	void Tick() override;
};
