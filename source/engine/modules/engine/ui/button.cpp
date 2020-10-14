#include "button.hpp"

#include <engine\state\state.hpp>

bool ui::button::process()
{
    ray::renderer_core_api::Renderer2D::DrawQuad({ this->render_data->Transform->Position.x, this->render_data->Transform->Position.y, 0}, this->render_data->Transform->Scale, this->render_data->RenderData->TextureId, this->graphics_context);
    
    return RayState()->Input->GetMousePosition().x >= this->render_data->Transform->Position.x && RayState()->Input->GetMousePosition().x <= this->render_data->Transform->Scale.x && RayState()->Input->GetMousePosition().y >= this->render_data->Transform->Position.y && RayState()->Input->GetMousePosition().y <= this->render_data->Transform->Scale.y;
}