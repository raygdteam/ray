#include "camera_actor.hpp"

RAYOBJECT_DESCRIPTION_BEGIN(CameraActor)
RAYOBJECT_DESCRIPTION_NAME("engine://camera")
RAYOBJECT_DESCRIPTION_END(CameraActor)

void CameraActor::UpdateMVP()
{
	Transform* transform = GetTransform();
	FVector3 pos = FVector3 { transform->Position.x, transform->Position.y, .0f};

	FMatrix4 translation = FMatrix4x4::Identity().Translate(pos) * FMatrix4x4::Identity().RotationX(0.f);
	
	// TODO: assumes 1280x720
	_projection = FMatrix4x4::Orthographic(1280, 720, .1f, 1.f);
	_view = translation.Inverse();
	_viewProjection = _projection * _view;
}

CameraActor::CameraActor()
{
	UpdateMVP();
}

