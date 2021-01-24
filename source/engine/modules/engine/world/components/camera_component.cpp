#include <engine/world/components/camera_component.hpp>
#include <engine/world/components/transform.hpp>
#include <engine/world/actor.hpp>

void CameraComponent::UpdateMVP()
{
	Transform* transform = _parent->GetTransform();
	FVector3 pos = FVector3{ transform->Position.x, transform->Position.y, .0f };

	FMatrix4x4 translation = FMatrix4x4::Identity().Translate(pos) * FMatrix4x4::Identity().RotationX(0.f);

	// TODO: assumes 1280x720
	_projection = FMatrix4x4::Orthographic(1280, 720, .1f, 1.f);
	_view = translation.Inverse();
	_viewProjection = _projection * _view;
}

RAYOBJECT_DESCRIPTION_BEGIN(CameraComponent)
RAYOBJECT_DESCRIPTION_NAME("engine://world/components/CameraComponent")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(CameraComponent)
