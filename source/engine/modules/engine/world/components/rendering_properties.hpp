#pragma once
#include <core/core.hpp>
#include <engine/world/components/component_base.hpp>
#include <core/math/vector.hpp>

struct MaterialRef
{
	String MaterialName;
};

class RenderingPropertiesComponent : public IComponent
{
	RAYOBJECT_BODY(RenderingComponent, IComponent);

	MaterialRef _materialRef;
public:
	void Init() override {}
	void Tick() override {}
	void OnDestroy() override {}
	
	void SetMaterialName(String& id);
	String& GetMaterialName();
	
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;

	void LoadFromJson(JsonValue& json) override;
};
