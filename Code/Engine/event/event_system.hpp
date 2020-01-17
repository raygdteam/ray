#pragma once
#include "pch.h"

namespace ray::event
{

class Event
{
	virtual ~Event() {};
};

template<typename EventWanted>
void on(const std::function<void(const Event&)>& fn);

void fire(const Event& _event);

}