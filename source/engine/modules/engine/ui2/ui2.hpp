#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/math/vector.hpp>
#include <engine/world/components/transform.hpp>

struct UiRenderData
{
	Transform* Transform;
	Array<FVector2> TexCoord;

	u32 TextureIndex;
};

class UiObject
{
	friend class UiRootObject;
	UiRenderData* _render_data;
	
protected:
	void Render();
	void Update();
public:
	const Array<UiObject&>& GetChildren() const noexcept;
	const UiObject& GetParent() const noexcept;
};

struct UiObjectsListEntry
{
	UiObject* Object;
	Transform** Transform;
	UiRenderData* RenderData;
};

class UiRootObject
{
	Array<UiObjectsListEntry> _objects;
public:
	void RenderAll();
	void AddObject(UiObject* root, UiObject*);
	const Array<UiObject&>& GetWindows();
};
