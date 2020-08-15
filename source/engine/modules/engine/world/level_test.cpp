#include "level.hpp"
#include <Windows.h>
#include <iostream>

class TestActor1 : public Actor
{
public:
	TestActor1()
	{
		GetTransform()->Position.x = 250.f;
		GetTransform()->Position.y = 10.f;
	}
	
protected:
	void BeginPlay() override
	{
		
	}
	
	void Tick(f64 delta) override
	{
		std::cout << "delta " << delta << "; fps " << 1000.f / delta << "\n";
		if (GetKeyState('D') & 0x8000)
		{
			GetTransform()->Position.x += 0.1f * delta;
		}
		if (GetKeyState('A') & 0x8000)
		{
			GetTransform()->Position.x -= 0.1f * delta;
		}

		if (GetKeyState('W') & 0x8000)
		{
			GetTransform()->Position.y -= 0.1f * delta;
		}
		
		if (GetKeyState('S') & 0x8000)
		{
			GetTransform()->Position.y += 0.1f * delta;
		}
	}
	
	void OnDestroy() override
	{
		
	}
};

class TestActor2 : public Actor
{
public:
	TestActor2()
	{
		GetTransform()->Position.x = 500.f;
		GetTransform()->Position.y = 10.f;
	}

protected:
	void BeginPlay() override
	{

	}

	void Tick(f64 delta) override
	{
		if (GetKeyState(VK_UP) & 0x8000)
		{
			GetTransform()->Position.y -= 0.1f * delta;
		}

		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			GetTransform()->Position.x -= 0.1f * delta;
		}

		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			GetTransform()->Position.y += 0.1f * delta;
		}
		
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			GetTransform()->Position.x += 0.1f * delta;
		}
	}

	void OnDestroy() override
	{

	}
};

void Level::LoadLevel()
{
	SpawnActor(new TestActor1());
	SpawnActor(new TestActor2());
}

void Level::Tick(f64 delta)
{
	for (u32 i = 0; i < _actors.Size(); ++i)
	{
		_actors[i]->Tick(delta);
	}
}
