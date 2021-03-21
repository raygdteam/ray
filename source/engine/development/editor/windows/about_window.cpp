#include "about_window.hpp"

#include "engine/ui2/objects/label.hpp"

static String gAboutWindowText;

EdAboutWindow::EdAboutWindow()
{
	Title = String("About Ray Engine");
	_objects.PushBack(new UiLabel(this, gAboutWindowText));

	bDestroyOnClose = true;
	Size = FVector2 { 250.f, 100.f };

	gAboutWindowText.clear();
	gAboutWindowText.append("Ray Game Engine\n");
	gAboutWindowText.append("  by Ray Team 2018 - 2021\n");
}

