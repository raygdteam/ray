#include "button.hpp"

bool ui::button::process()
{
    //ray::renderer_core_api::Renderer2D::DrawQuad({ this->transform->Position.x, this->transform->Position.y, 0}, this->transform->Scale, textureCoords, gfxContext);

    return input::mouse::x() >= this->transform->Position.x && input::mouse::x() <= this->transform->Scale.x && input::mouse::y() >= this->transform->Position.y && input::mouse::y() <= this->transform->Scale.y;
}