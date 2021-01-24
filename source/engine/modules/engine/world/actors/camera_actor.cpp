#include "camera_actor.hpp"
#include <engine/state/state.hpp>
#include <input/input.hpp>

RAYOBJECT_DESCRIPTION_BEGIN(CameraActor)
RAYOBJECT_DESCRIPTION_NAME("engine://world/actors/CameraActor")
RAYOBJECT_DESCRIPTION_END(CameraActor)

CameraActor::CameraActor()
{
	_cameraComponent = new CameraComponent;
	AddComponent(_cameraComponent);
	_cameraComponent->UpdateMVP();
}

void CameraActor::Tick(f64 dt)
{
	FVector2 movement = {};
	bool changed = false;
	Input* input = RayState()->Input;

	if (input->IsKeyDown(Input::eD))
	{
		movement.x += .01f * dt;
		changed = true;
	}
	if (input->IsKeyDown(Input::eS))
	{
		movement.y -= .01f * dt;
		changed = true;
	}
	if (input->IsKeyDown(Input::eA))
	{
		movement.x -= .01f * dt;
		changed = true;
	}
	if (input->IsKeyDown(Input::eW))
	{
		movement.y += .01f * dt;
		changed = true;
	}

	if (changed)
	{
		// GetTransform()->Position += movement;
		GetTransform()->Position.x += movement.x;
		GetTransform()->Position.y += movement.y;
		_cameraComponent->UpdateMVP();
	}
}

