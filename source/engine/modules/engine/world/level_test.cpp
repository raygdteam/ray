#include "level.hpp"
#include <Windows.h>
#include <iostream>
#include <engine/state/state.hpp>
#include <core/file_system/file_system.hpp>

#include <engine/bundle/bundle_meta.hpp>

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
		// std::cout << "delta " << delta << "; fps " << 1000.f / delta << "\n";
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

#define RAY_ACTOR_DATATYPE 0x1
#define RAY_LEVEL_DATATYPE 0x2

/*
 *	RayLevelBundle				|	BundleHeader size=24=0x18
 *		Bundle					|	Bundle? size=???
 *			BundleHeader		|	NumActors = size=8
 *	NumActors					|
 *								|
 *	///////////////////////////	|	////////////////
 *								|
 *	ActorBundleData * NumActors	|	ActorBundleData * NumActors	
 */

/*
 * World? physics? interaction between loaded and unloaded level? assume none?
 * static geometry always loaded?
 */

struct RayLevelBundle
{
	BundleHeader Header;
	u64 NumActors;
};

struct ActorBundleData
{
	Transform Transform;
	// other components? refl? manual serialization? constant size? alignment?
};

void Level::LoadLevel()
{
	/*IFile* bundle = ray::RayState()->FileSystem->OpenFile("../../test.bundle", Write);

	Bundle bundleFile = {
		.Header = {
			.Magic = reinterpret_cast<u64>("RAY!"),
			.Datatype = 32,
			.Checksum = 0xf0f0f0f0f0f0f0f0,
		}
	};

	auto written = bundle->Write<Bundle>(bundleFile);
	(void)written;
	
	bundle->Close();
	delete bundle;*/
	/*
	IFile* bundle2 = ray::RayState()->FileSystem->OpenFile("../../test.bundle", Read);

	Bundle file2 = {};
	bundle2->Read(file2);
	
	bundle2->Close();
	delete bundle2;
	*/

	/*RayLevelBundle bundleFile = {
		.Header = {
			.Magic = 0xA0B1C2D3E4F50000ULL,
			.Datatype = 32,
			.Checksum = 0xf0f0f0f0f0f0f0f0,
		},
		.NumActors = 2
	};
	ActorBundleData actorData1 = {
		// nicer way?
		.Transform = Transform(FVector<2> {(f64)100.f, (f64)100.f})
	};
	ActorBundleData actorData2 = {
		// nicer way?
		.Transform = Transform(FVector<2> {(f64)500.f, (f64)100.f})
	};*/

	// verify header on load?
	
	/*
	 * 00 00 F5 E4 D3 C2 B1 A0			Magic
	 * 20 00 00 00 00 00 00 00			Datatype
	 * F0 F0 F0 F0 F0 F0 F0 F0			Checksum
	 * 02 00 00 00 00 00 00 00			NumActors
	 *
	 * Actor 1	(vtable?)			100.f						100.f
	 * 48 86 A0 69 FD 7F 00 00		00 00 00 00 00 00 59 40		00 00 00 00 00 00 59 40
	 *
	 * Actor 2	(vtable?)			500.f						100.f
	 * 48 86 A0 69 FD 7F 00 00		00 00 00 00 00 40 7F 40		00 00 00 00 00 00 59 40
	 */
	
	/*
	 * 00 00 F5 E4 D3 C2 B1 A0
	 * 20 00 00 00 00 00 00 00
	 * F0 F0 F0 F0 F0 F0 F0 F0
	 * 02 00 00 00 00 00 00 00
	 *
	 * 48 86 84 69 FD 7F 00 00 00 00 00 00 00 00 59 40
	 * 00 00 00 00 00 00 59 40 48 86 84 69 FD 7F 00 00 00 00 00 00 00 40 7F 40
	 * 00 00 00 00 00 00 59 40
	 */

	/*
	 * vtable? obj sys?
	 */
	
	/*IFile* bundle = ray::RayState()->FileSystem->OpenFile("../../test.bundle", Write);

	static_assert(sizeof(RayLevelBundle) == 32, "no");
	
	bundle->Write(bundleFile);
	bundle->Write(actorData1);
	bundle->Write(actorData2);
	
	bundle->Close();
	delete bundle;*/

	IFile* bundle = ray::RayState()->FileSystem->OpenFile("../../test.bundle", Read);
	
	RayLevelBundle bundleFile = {};
	ActorBundleData actor1 = {};
	ActorBundleData actor2 = {};
	
	/*
	 * !!!!!!!!!!undefined behaviour!!!!!!!!!!!!!!!!!!
	 */
	bundle->Read(bundleFile);
	bundle->Read(actor1);
	bundle->Read(actor2);

	bundle->Close();
	delete bundle;

	if ((bundleFile.Header.Magic != 0xA0B1C2D3E4F50000ULL) ||
		(bundleFile.Header.Checksum != 0xf0f0f0f0f0f0f0f0) ||
		(bundleFile.Header.Datatype != 32) ||
		(bundleFile.NumActors != 2))
	{
		*(int*)nullptr = 0xDED;
	}

	TestActor1* actor_1 = new TestActor1();
	actor_1->GetTransform()->Position = actor1.Transform.Position;
	SpawnActor(actor_1);

	TestActor2* actor_2 = new TestActor2();
	actor_2->GetTransform()->Position = actor2.Transform.Position;
	SpawnActor(actor_2);
}

void Level::Tick(f64 delta)
{
	for (u32 i = 0; i < _actors.Size(); ++i)
	{
		_actors[i]->Tick(delta);
	}
}
