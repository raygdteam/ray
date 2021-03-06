#include "debug_window.hpp"

#include "engine/ui2/objects/label.hpp"

EdDebugWindow::EdDebugWindow()
{
	String text("AAAAAAAAAAAAAAAAAAAA");
	this->_objects.PushBack(new UiLabel(text));
}

