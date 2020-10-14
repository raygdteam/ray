#pragma once

class UIObject
{
public:
	UIObject();
	~UIObject() {};

	virtual void Tick() = 0;
	virtual void Render() = 0;
};