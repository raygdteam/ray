#include "pch.hpp"
#include "startup_initializer.hpp"

namespace ray::core::modules
{

struct InitializationPhaseBinding
{
	string name;
	InitializationPhase phase;
	std::function<void()> function;
};

std::vector<InitializationPhaseBinding> InitializationManager::_bindings;

void InitializationManager::RegisterPhase(string name, InitializationPhase phase, std::function<void()> function)
{
	// Ensure unique
	for (auto binding : _bindings)
		if (name == binding.name) return;

	_bindings.push_back({ name, phase, function });
}

void InitializationManager::RunPhase(InitializationPhase phase)
{
	for (auto binding : _bindings)
		if (binding.phase == phase)
			binding.function();
}

RegisterStartupPhase::RegisterStartupPhase(string name, InitializationPhase phase, const std::function<void()>& function)
{
	InitializationManager::RegisterPhase(name, phase, function);
}

}
