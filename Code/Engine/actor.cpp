#include "pch.h"
#include "actor.hpp"
#include "IComponent.hpp"

namespace ray
{
	//  Хлава сатанам, если тут нет ошибок
	template<class T, class... Params>
	void Actor::add_component(Params&&... args)
	{
		//  Хлава сатанам, если тут нет ошибок
		static_assert(std::is_base_of<IComponent, T>, 
			"add_component<..> requires class to be extended from IComponent");

		auto component = std::make_shared<T>(args);
		_components.push_back(component);
	}

	void Actor::Destroy()
	{
		_components.clear();
	}
}