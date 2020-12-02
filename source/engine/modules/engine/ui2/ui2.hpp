#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>
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
	UiObject* _parent;
	Array<UiObject*> _children;
	
protected:
	void Render() {}
	void Update() {}
public:
	UiObject();
	
	const Array<UiObject*>& GetChildren() const noexcept
	{
		return _children;
	}

	const UiObject& GetParent() const noexcept
	{
		return *_parent;
	}
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
	FMatrix4x4 _vp;
public:
	void RenderAll();
	void AddObject(UiObject* root, UiObject*);
	const Array<UiObject&>& GetWindows();
};
