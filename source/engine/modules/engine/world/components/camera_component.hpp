#pragma once
#include <engine/world/components/component_base.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>

class CameraComponent : public IComponent
{
	RAYOBJECT_BODY(CameraComponent, IComponent);

	FMatrix4x4 _projection;
	FMatrix4x4 _view;
	FMatrix4x4 _viewProjection;
public:

	void Init() override { UpdateMVP(); }
	void Tick() override {}
	void OnDestroy() override {}
	void Serialize(Archive&) override {}
	void Deserialize(Archive&) override {}
	
	void UpdateMVP();

	const FMatrix4x4& GetProjection() { return _projection; }
	const FMatrix4x4& GetView() { return _view; }
	const FMatrix4x4& GetViewProjection() { return _viewProjection; }
};
