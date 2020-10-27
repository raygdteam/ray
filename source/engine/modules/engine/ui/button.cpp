#include <engine/ui/button.hpp>

void UiButton::Tick()
{
	
}

void UiButton::Render()
{
}

UiButton::UiButton()
{
	_transform = new Transform;
	UiRenderData* ctx = _renderData = new UiRenderData;

	ctx->Position = FVector3 { _transform->Position.x, _transform->Position.y, 1.f };
	ctx->Scale = FVector2 { 1.f, 1.f };

	ctx->TexCoord.PushBack(FVector2 {-1.f, -1.f});
	ctx->TexCoord.PushBack(FVector2 {1.f, -1.f});
	ctx->TexCoord.PushBack(FVector2 {1.f, 1.f});
	ctx->TexCoord.PushBack(FVector2 {-1.f, 1.f});
	
	ctx->TextureIndex = 0;
}
