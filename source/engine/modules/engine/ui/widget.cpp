#include "widget.hpp"

void UIWidget::AddObject(UIObject& object)
{
	this->_objects.PushBack(&object);
}

void UIWidget::RemoveObject(UIObject& object)
{
	// ?
}