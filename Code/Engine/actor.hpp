#pragma once
#include "imodel_component.hpp"
#include "object.hpp"
#include "IComponent.hpp"

namespace ray
{
	class Actor : public object
	{
		GENERATED_BODY;
	public:
		void Destroy();
		bool CanTick()
		{ return bCanEverTick; }
		virtual void Tick(float delta) = 0;
		std::vector<std::shared_ptr<IComponent>> GetComponents()
		{ return _components; }

	protected:
		template<class T, class... Params>
		void add_component(Params&&... args);
	private:
		std::vector<std::shared_ptr<IComponent>> _components;
		bool bCanEverTick = true;
	};
}
