#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <functional>

struct RayObject;

enum FieldType
{
	eU8 = 0x0,
	eU16,
	eU32,
	eU64,

	eS8,
	eS16,
	eS32,
	eS64,

	eF32,
	eF64,

	eBoolean,
	eString,

	eVector1,
	eVector2,
	eVector3,
	eVector4,
};

struct FieldInfo
{
	FieldType Type;
	u32 Offset;
	u32 Size;
};

struct Type
{
	const char* Name;
	u32 NameCrc32 = 0;
	Array<FieldInfo> Fields;

	bool Abstract = true;
	std::function<RayObject* ()> CreateFn;

	RayObject* CreateInstance()
	{
		return CreateFn();
	}
};
