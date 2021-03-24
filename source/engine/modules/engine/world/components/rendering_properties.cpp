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

void RenderingPropertiesComponent::LoadFromJson(JsonValue& json)
{
	MaterialName = json["material_name"].AsString();
}

RAYOBJECT_DESCRIPTION_BEGIN(RenderingPropertiesComponent)
RAYOBJECT_DESCRIPTION_NAME("engine://world/components/RenderingProperties")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_FIELD(MaterialName, String, eString);
RAYOBJECT_DESCRIPTION_END(RenderingPropertiesComponent)
