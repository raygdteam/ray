#include "rendering_properties.hpp"

#include "engine/world/actor.hpp"
#include "engine/world/world.hpp"

void RenderingPropertiesComponent::Init()
{
	if (Material != nullptr) return;

	String name("/game/materials/default");
	Material = _world->GetMaterialInstance(_world->GetMaterialIdForName(name)).Material;
}

void RenderingPropertiesComponent::SetMaterialName(String& id)
{
	//Material = _parent.GetWorld().CompileMaterial()
}

String& RenderingPropertiesComponent::GetMaterialName()
{
	return Material->GetName();
}

void RenderingPropertiesComponent::Serialize(Archive&)
{
}

void RenderingPropertiesComponent::Deserialize(Archive&)
{
}

void RenderingPropertiesComponent::LoadFromJson(DictionaryValue& value)
{
	String name(value.AsObject()["material_name"].AsString());
	Material = _world->GetMaterialInstance(_world->GetMaterialIdForName(name)).Material;
}

RAYOBJECT_DESCRIPTION_BEGIN(RenderingPropertiesComponent)
RAYOBJECT_DESCRIPTION_NAME("engine://world/components/RenderingProperties")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_FIELD(Material, RMaterialInstance*, eResourceRef);
RAYOBJECT_DESCRIPTION_END(RenderingPropertiesComponent)
