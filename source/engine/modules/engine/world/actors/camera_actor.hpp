#pragma once
#include <engine/world/actor.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>
#include <engine/world/components/camera_component.hpp>

class CameraActor : public Actor
{
	friend class RayEngine;
	RAYOBJECT_BODY(CameraActor, Actor);

	CameraComponent* _cameraComponent = nullptr;

protected:
	void MoveTo(FVector2 pos) override
	{
		Super::MoveTo(pos);
		_cameraComponent->UpdateMVP();
	}
public:
	CameraActor();

	void Awake() override { ATD.Stage = ePrePhysicsUpdate; ATD.Active = true; }
	void BeginPlay() override {}
	void Tick(f64) override;
	void OnDestroy() override {}

	CameraComponent* GetCameraComponent()
	{
		return _cameraComponent;
	}
};
