#pragma once

struct IComponent
{
	virtual ~IComponent() = default;
	
	virtual void Init() = 0;
	virtual void Tick() = 0;
	virtual void OnDestroy() = 0;
};
