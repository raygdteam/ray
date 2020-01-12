#pragma once
#include <functional>

enum class EventType
{
	WINDOW_CLOSE, WINDOW_RESIZE,
	KEY_PRESSED, KEY_RELEASED
};

class Event
{
public:
	Event() {}
	~Event() {}

	virtual EventType GetEventType() = 0;

	bool handled;

};


class EventDispatcher
{
public:
	EventDispatcher(Event& e) : m_Event(e) {}

	template <typename Type, typename Func>
	bool Dispatch(const Func& f)
	{
		if (this->m_Event.GetEventType() == Type::GetStaticType())
		{
			this->m_Event.handled = f(static_cast<Type&>(this->m_Event));
			return true;
		}
		return false;
	}

private:
	Event& m_Event;

};

using EventCallback = std::function<void(Event&)>;