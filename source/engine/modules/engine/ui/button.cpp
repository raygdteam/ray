#include "button.hpp"

// ray::renderer_core_api::Renderer2D::DrawQuad(pos, size, textureCoords, gfxContext);

bool ui::button::process()
{
    return input::mouse::x() >= this->position.x && input::mouse::x() <= this->size.x && input::mouse::y() >= this->position.y && input::mouse::y() <= this->size.y;
}