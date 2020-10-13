#include <core/core.hpp>
#include <core/lib/array.hpp>

/*

Coordinate system

   (1, 0)				  (1, 1)
	+-----------------------+
	|						|
	|						|
	|						|
	|						|
	|						|
	+-----------------------+
  (0, 0)				 (0, 1)


Every Object's coordinate is relative to it's parent Widget unless explicitly stated
*/

class UiObject
{
	struct RenderData* _renderData;
	struct Transform* _transform;
};

struct UiObjectProxy
{
	struct RenderData** _renderData;
	struct Transform** _transform;
	// ...
};

class UiWidget
{
	Array<UiObject> _objects;
	Array<UiObjectProxy> _proxies;

	void AddObject(UiObject*);
	void RemoveObject(UiObject*); // ?

	void ProcessTick()
	{
		/*
		 * foreach object in _objects
		 * {
		 *		if button
		 *			process as button
		 *		else if ...
		 *			...
		 *
		 *		Render();
		 * }
		 */
	}
};
