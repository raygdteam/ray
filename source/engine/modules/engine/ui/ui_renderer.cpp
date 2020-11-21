#include <core/core.hpp>
#include <engine/ui/widget.hpp>
#include <engine/ui/ui_render_data.hpp>
#include "core/math/matrix.hpp"

class UiRenderer
{
	Array<UiObject*> _objects;
	Array<UiObjectProxy> _proxies;

	UiObject* GetRootObject();
	FVector2 WorldToScreen(FVector2& world, FMatrix4x4& viewProjection);
	
	void Render()
	{
		// Assuming (0, 0) is top left,
		// with +X downward and +Y right-ward.

		// Get root object
		UiObject* root = GetRootObject();

		// Calculate view projection
		FMatrix4x4 viewProjection;

		
	}
};
