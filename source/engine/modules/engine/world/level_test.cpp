#include "level.hpp"
#include <Windows.h>
#include <iostream>
#include <engine/state/state.hpp>
#include <core/file_system/file_system.hpp>
#include <core/object/file_archive.hpp>
#include <core/object/object.hpp>

#include <engine/bundle/bundle_meta.hpp>


class TestActor1 : public Actor
{
	RAYOBJECT_BODY(TestActor1, Actor);
	
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

public:
	void Serialize(Archive& ar) override
	{
		Super::Serialize(ar);
	}

	void Deserialize(Archive& ar) override
	{
		Super::Deserialize(ar);
	}
};

class TestActor2 : public Actor
{
	RAYOBJECT_BODY(TestActor2, Actor);
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
	
public:
	void Serialize(Archive& ar) override
	{
		Actor::Serialize(ar);
	}

	void Deserialize(Archive& ar) override
	{
		Actor::Deserialize(ar);
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

RAYOBJECT_DESCRIPTION_BEGIN(TestActor1)
	RAYOBJECT_DESCRIPTION_CREATEABLE();
	RAYOBJECT_DESCRIPTION_NAME("engine://world/level_test/TestActor1");
RAYOBJECT_DESCRIPTION_END(TestActor1)

RAYOBJECT_DESCRIPTION_BEGIN(TestActor2)
	RAYOBJECT_DESCRIPTION_CREATEABLE();
	RAYOBJECT_DESCRIPTION_NAME("engine://world/level_test/TestActor2");
RAYOBJECT_DESCRIPTION_END(TestActor2)

void Level::LoadLevel()
{
	/*
	 * 00 00 F5 E4 D3 C2 B1 A0			Magic
	 * 20 FE FE FE FE FE FE FE			Datatype (whytf FE FE FE FE FE???)
	 * F0 F0 F0 F0 F0 F0 F0 F0			Checksum
	 * 02 00 00 00 00 00 00 00			NumActors
	 *
	 *	Actor #1
	 * 
	 * D0 22 24 C0						TestActor1 type CRC
	 * 01 00 00 00 00 00 00 00			Actor component size
	 * 00 00 00 00 00 40 6F 40			Transform.Position.x
	 * 00 00 00 00 00 00 24 40			Transform.Position.y
	 *
	 *	Actor #2
	 *
	 * 24 D1 74 D3						TestActor2 type CRC			
	 * 01 00 00 00 00 00 00 00			Actor component size
	 * 00 00 00 00 00 40 7F 40			Transform.Position.x
	 * 00 00 00 00 00 00 24 40			Transform.Position.y
	 */

	
	/*SpawnActor(new TestActor1());
	SpawnActor(new TestActor2());
	
	RayLevelBundle bundleFile = {
		.Header = {
			.Magic = 0xA0B1C2D3E4F50000ULL,
			.Datatype = 32,
			.Checksum = 0xf0f0f0f0f0f0f0f0,
		},
		.NumActors = _actors.Size()
	};

	IFile* bundle = ray::RayState()->FileSystem->OpenFile("../../test.bundle", Write);
	FileArchive ar;
	ar.file = bundle;

	bundle->Write(bundleFile);
	for (Actor* actor : _actors)
	{
		actor->Serialize(ar);
	}
	
	bundle->Close();
	delete bundle;*/

	IFile* bundle = RayState()->FileSystem->OpenFile("../../test.bundle", Read);
	FileArchive ar;
	ar.file = bundle;

	RayLevelBundle bundleFile = {};
	bundle->Read(bundleFile);

	_actors.clear();

	for (u64 i = 0; i < bundleFile.NumActors; ++i)
	{
		u32 object = 0;
		bundle->Read<u32>(object);

		Type* objectType = RayState()->ObjectDb->GetTypeByCrc(object);
		if (objectType == nullptr || objectType->Abstract)
		{
			// TODO: fail
		}

		RayObject* objectInstance = objectType->CreateInstance();
		Actor* actor = nullptr;

		// If not actor then buntovat'
		actor = dynamic_cast<Actor*>(objectInstance);
		if (actor == nullptr)
		{
			// TODO: fail
		}

		// Pray to god we're right
		actor->Deserialize(ar);

		SpawnActor(actor);
	}
	
	bundle->Close();
	delete bundle;
	
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
}

void Level::Tick(f64 delta)
{
	for (Actor* actor : _actors)
	{
		actor->Tick(delta);
	}
}
