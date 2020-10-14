#include "manager.hpp"

void UiManager::tick()
{
	for (size_t index = 0; index < this->_objects.size(); index++)
		this->_objects[index]->tick();
}

void UiManager::render()
{
	for (size_t index = 0; index < this->_objects.size(); index++)
		this->_objects[index]->render();
}