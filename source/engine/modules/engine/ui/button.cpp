#include "button.hpp"

#include <renderer_core/renderer_2d.hpp>

#include <engine/state/state.hpp>

void UIButton::Tick()
{
    if (RayState()->Input->GetMouseDelta().x >= this->Transform->Position.x && RayState()->Input->GetMousePosition().x <= this->Transform->Scale.x && RayState()->Input->GetMousePosition().y >= this->Transform->Position.y && RayState()->Input->GetMousePosition().y <= this->Transform->Scale.y)
        ;
}

void UIButton::Render()
{
    ray::renderer_core_api::Renderer2D::DrawQuad({ this->Transform->Position.x, this->Transform->Position.y, 0 }, this->Transform->Scale, this->TextureId, ray::renderer_core_api::GraphicsContext::Begin());
}