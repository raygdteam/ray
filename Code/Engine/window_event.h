#pragma once
#include "Event.hpp"

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}
	static EventType GetStaticType() { return EventType::WINDOW_CLOSE; }
	EventType GetEventType() { return GetStaticType(); }
};