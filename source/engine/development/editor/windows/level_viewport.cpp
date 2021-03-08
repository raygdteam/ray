#include "level_viewport.hpp"
#include "engine/ui2/objects/image.hpp"

EdLevelViewport::EdLevelViewport()
{
	Title = String("Level Viewport");
	this->_objects.PushBack(new UiImage());
}

