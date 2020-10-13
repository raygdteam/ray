#include "button.hpp"

// ray::renderer_core_api::Renderer2D::DrawQuad(pos, size, textureCoords, gfxContext);

bool ui::button::process()
{
    return input::mouse::x() >= this->transform->Position.x && input::mouse::x() <= this->transform->Scale.x && input::mouse::y() >= this->transform->Position.y && input::mouse::y() <= this->transform->Scale.y;
}