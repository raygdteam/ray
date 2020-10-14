#pragma once

#include "object.hpp"

#include <engine\world\components\transform.hpp>

class UIButton : protected UIObject
{
	UIButton(Transform* Transform, u64 TextureId)
	{
		this->Transform = Transform;
		this->TextureId = TextureId;
	}

	~UIButton();

	virtual void Tick() override;
	virtual void Render() override;

private:
	Transform* Transform;
	u64 TextureId;
};