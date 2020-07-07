#pragma once

namespace ray::input
{
	class Listener
	{
	public:
		Listener() {}
		~Listener() = default;

		// keyboard pure virtual callback functions 
		virtual void OnKeyDown(int) = 0;
		virtual void OnKeyUp(int) = 0;

		// mouse pure virtual callback functions
		virtual void OnMouseMove(int, int) = 0;

		virtual void OnLeftMouseDown(int, int) = 0;
		virtual void OlLeftMouseUp(int, int) = 0;

		virtual void OnRightMouseDown(int, int) = 0;
		virtual void OnRightMouseUp(int, int) = 0;
	};
}