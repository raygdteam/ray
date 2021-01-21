#include "camera_actor.hpp"
#include <engine/state/state.hpp>
#include <input/input.hpp>

RAYOBJECT_DESCRIPTION_BEGIN(CameraActor)
RAYOBJECT_DESCRIPTION_NAME("engine://camera")
RAYOBJECT_DESCRIPTION_END(CameraActor)

void CameraActor::UpdateMVP()
{
	Transform* transform = GetTransform();
	FVector3 pos = FVector3 { transform->Position.x, transform->Position.y, .0f};

	FMatrix4x4 translation = FMatrix4x4::Identity().Translate(pos) * FMatrix4x4::Identity().RotationX(0.f);
	
	// TODO: assumes 1280x720
	_projection = FMatrix4x4::Orthographic(1280, 720, .1f, 1.f);
	_view = translation.Inverse();
	_viewProjection = _projection * _view;
}

CameraActor::CameraActor()
{
	UpdateMVP();
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
		UpdateMVP();
	}
}

