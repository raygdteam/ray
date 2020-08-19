#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <functional>

struct RayObject;

enum FieldType
{
	U8 = 0x0,
	U16,
	U32,
	U64,

	S8,
	S16,
	S32,
	S64,

	F32,
	F64,

	Boolean,
	String,

	Vector1,
	Vector2,
	Vector3,
	Vector4,
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
