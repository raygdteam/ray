#pragma once
#include "core/pch.hpp"

namespace ray::core::modules
{

enum class InitializationPhase : u16
{
	eStartupPlugin,
	ePreInitialization,
	eInitialization,
	ePostInitialization
};

struct RAY_CORE_API RegisterStartupPhase
{
	RegisterStartupPhase(string name, InitializationPhase, const std::function<void()>&);
};

struct InitializationPhaseBinding;

class RAY_CORE_API InitializationManager
{
	// фу! плохо!
	static std::vector<InitializationPhaseBinding> _bindings;
public:
	static void RegisterPhase(string name, InitializationPhase, std::function<void()>);
	static void RunPhase(InitializationPhase);
};

}