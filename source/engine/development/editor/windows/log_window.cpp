#include "log_window.hpp"
#include <core/log/log.hpp>
#include <engine/ui2/objects/label.hpp>

EdLogWindow::EdLogWindow()
{
	Title = String("Log");
	//_objects.PushBack(new UiLabel(this, GetLogsAll()));
}

void EdLogWindow::Tick()
{
}

