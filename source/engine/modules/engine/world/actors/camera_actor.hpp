#pragma once
#include <engine/world/actor.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>

class CameraActor : public Actor
{
	RAYOBJECT_BODY(CameraActor, Actor);

	FMatrix4x4 _projection;
	FMatrix4x4 _view;
	FMatrix4x4 _viewProjection;

	void UpdateMVP();
protected:
	void MoveTo(FVector2 pos) override
	{
		Super::MoveTo(pos);
		UpdateMVP();
	}
public:
	void BeginPlay() override {}
	void Tick(f64) override {}
	void OnDestroy() override {}

	const FMatrix4x4& GetProjection() { return _projection; }
	const FMatrix4x4& GetView() { return _view; }
	const FMatrix4x4& GetViewProjection() { return _viewProjection; }
};
