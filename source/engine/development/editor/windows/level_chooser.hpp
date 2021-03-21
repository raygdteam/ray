#pragma once
#include <engine/ui2/ui2.hpp>

class LevelChooser : public UiWindow
{
public:
	LevelChooser();
	~LevelChooser() = default;
	void Tick() override;
};
