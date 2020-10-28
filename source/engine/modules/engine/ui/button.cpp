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

	ctx->Position = FVector3 { 400.f, 600.f, 1.f };
	ctx->Scale = FVector2 { 1.f, 1.f };

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
