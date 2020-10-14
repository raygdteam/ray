#include "manager.hpp"

void UIManager::Tick()
{
	for (size_t index = 0; index < this->_objects.size(); index++)
		this->_objects[index]->Tick();
}

void UIManager::Render()
{
	for (size_t index = 0; index < this->_objects.size(); index++)
		this->_objects[index]->Render();
}