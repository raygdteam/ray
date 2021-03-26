#include "rendering_properties.hpp"

void RenderingPropertiesComponent::SetMaterialName(String& id)
{
	MaterialName = id;
}

String& RenderingPropertiesComponent::GetMaterialName()
{
	return MaterialName;
}

void RenderingPropertiesComponent::Serialize(Archive&)
{
}

void RenderingPropertiesComponent::Deserialize(Archive&)
{
}

void RenderingPropertiesComponent::LoadFromJson(ray::json::value& json)
{
	MaterialName = String(json.as_dictionary()["material_name"].as_string());
}

RAYOBJECT_DESCRIPTION_BEGIN(RenderingPropertiesComponent)
RAYOBJECT_DESCRIPTION_NAME("engine://world/components/RenderingProperties")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_FIELD(MaterialName, String, eString);
RAYOBJECT_DESCRIPTION_END(RenderingPropertiesComponent)
