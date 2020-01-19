#pragma once
#include <pch.hpp>
#include <cstdlib>

namespace ray::component
{

struct IComponentBase
{
	virtual ~IComponentBase() = default;


	virtual void BeginPlay() = 0;
	virtual void Tick(float delta) = 0;
	virtual void Destroy() = 0;
};

struct IComponentUtility : public IComponentBase
{
protected:
	
};

using IComponent = std::shared_ptr<IComponentUtility>;

struct IComponentRenderable
{
	
};

}