#include "widget.hpp"

void UiWidget::AddObject(ui::object& object)
{
	this->_objects.PushBack(&object);
}

void UiWidget::RemoveObject(ui::object& object)
{
	// ?
}