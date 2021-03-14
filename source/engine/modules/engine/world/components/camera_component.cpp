#include <engine/world/components/camera_component.hpp>
#include <engine/world/components/transform.hpp>
#include <engine/world/actor.hpp>

#include <DirectXMath.h>
using namespace DirectX;

void CameraComponent::UpdateMVP()
{
	Transform* transform = _parent->GetTransform();
	FVector3 pos = FVector3 { -transform->Position.x, -transform->Position.y, .0f };


	// TODO: assumes 1280x720
	FMatrix4x4 translation = FMatrix4x4::Identity().Translate(pos) * FMatrix4x4::Identity().RotationX(0.f);
	_projection = FMatrix4x4::Orthographic(1280, 720, .1f, 1.f);
	_view = translation.Inverse();
	_viewProjection = _view * _projection;
	
	//XMFLOAT3 xmpos = { pos.x, pos.y, pos.z };
	//XMFLOAT3 xmfocus = { 0,0,1 };
	//XMFLOAT3 xmup = { 0,1,0 };
	//XMMATRIX dview = XMMatrixLookAtLH(XMLoadFloat3(&xmpos), XMLoadFloat3(&xmfocus), XMLoadFloat3(&xmup));
	//XMMATRIX dproj = XMMatrixOrthographicLH(1280, 720.f, 0.1f, 1.6f);
	////XMMATRIX vp = dproj * dview;
	//XMMATRIX vp = dview * dproj;
	//XMFLOAT4X4 res = {};
	//XMStoreFloat4x4(&res, vp);
	//_viewProjection = (*(FMatrix4x4*)&res);
}

RAYOBJECT_DESCRIPTION_BEGIN(CameraComponent)
RAYOBJECT_DESCRIPTION_NAME("engine://world/components/CameraComponent")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(CameraComponent)
