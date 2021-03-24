#include "rendering_properties.hpp"

void RenderingPropertiesComponent::SetMaterialName(String& id)
{
	_materialRef.MaterialName = id;
}

String& RenderingPropertiesComponent::GetMaterialName()
{
	return _materialRef.MaterialName;
}

void RenderingPropertiesComponent::Serialize(Archive&)
{
}

void RenderingPropertiesComponent::Deserialize(Archive&)
{
}

void RenderingPropertiesComponent::LoadFromJson(ray::json::value& json)
{
	_materialRef.MaterialName = String(json.as_dictionary()->operator[]("material_name").as_string());
}

RAYOBJECT_DESCRIPTION_BEGIN(RenderingPropertiesComponent)
RAYOBJECT_DESCRIPTION_NAME("engine://world/components/RenderingProperties")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(RenderingPropertiesComponent)
