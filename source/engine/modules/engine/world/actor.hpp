#pragma once
#include <core/object/object.hpp>
#include <engine/world/components/transform.hpp>

namespace ray
{

class Actor : public RayObject
{
	Transform _transform;
protected:
	virtual void BeginPlay() = 0;
	virtual void Tick(f64 delta) = 0;
	virtual void OnDestroy() = 0;
public:
	Actor();
	virtual ~Actor();

	Transform GetTransform();
};
	
}

#define TEST(x) __pragma(code_seg())
