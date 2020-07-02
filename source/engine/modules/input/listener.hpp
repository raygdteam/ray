#pragma once

namespace ray::input
{
	class listener
	{
	public:
		listener() {}
		~listener() = default;

		// keyboard pure virtual callback functions 
		virtual void on_key_down(int) = 0;
		virtual void on_key_up(int) = 0;

		// mouse pure virtual callback functions
		virtual void on_mouse_move(int, int) = 0;

		virtual void on_left_mouse_down(int, int) = 0;
		virtual void on_left_mouse_up(int, int) = 0;

		virtual void on_right_mouse_down(int, int) = 0;
		virtual void on_right_mouse_up(int, int) = 0;
	};
}