#include <engine/ui/button.hpp>
#include <engine/state/state.hpp>
#include <input/input.hpp>

void UiButton::Tick()
{
	InputBase* input = RayState()->Input;
	FVector2 cursor = input->GetMouseDelta();
	_transform->Position = _transform->Position + cursor.Multiply(1.0f);
	/*FVector2 myPos = _parent->_transform->Position + _transform->Position;
	FVector2 size = _transform->Scale;

	if (myPos.x < cursor.x &&
		myPos.y < cursor.y &&
		myPos.x + size.x > cursor.x &&
		myPos.y + size.y > cursor.y)
	{
		__debugbreak();
	}*/
}

void UiButton::Render()
{
}

UiButton::UiButton()
{
	_transform = new Transform;
	UiRenderData* ctx = _renderData = new UiRenderData;

	_transform->Position = FVector2 { 0.f, 0.f };
	_transform->Scale = FVector2 { 1.f, 1.f };

	ctx->Transform = _transform;
	
	static FVector<2> textureCoords[4] =
	{
		{ 0.f, 1.f },
		{ 0.f, 0.f },
		{ 1.f, 0.f },
		{ 1.f, 1.f }
	};

	ctx->TexCoord.PushBack(textureCoords[0]);
	ctx->TexCoord.PushBack(textureCoords[1]);
	ctx->TexCoord.PushBack(textureCoords[2]);
	ctx->TexCoord.PushBack(textureCoords[3]);
	
	ctx->TextureIndex = 0;
}
