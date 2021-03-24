#pragma once
#include <core/object/archive.hpp>
#include <core/object/type.hpp>
#include <core/json/json.hpp>

#include "core/lib/json.hpp"

struct RayObject
{
	RayObject() {}
	virtual ~RayObject() {}

	static Type* GetStaticType();
	virtual Type* GetType() = 0;

	virtual void Serialize(Archive&) = 0;
	virtual void Deserialize(Archive&) = 0;

	virtual void LoadFromJson(JsonValue& json) {}
};

class RAY_CORE_API ObjectDb
{
	Array<Type*> _objects;
public:
	ObjectDb();

	Type* GetTypeByName(pcstr name);
	Type* GetTypeByCrc(u32 crc);
	
	Array<Type*>& GetAllTypes();

	static void __Internal_RegisterObjectStatic(Type*);
};


#define RAYOBJECT_BODY(TType, Base) using Super = Base; \
	friend struct AUTOREGISTER__##TType; \
	public: \
	static ::Type* GetStaticType(); \
	::Type* GetType() override; \
	private:

#define RAYOBJECT_DESCRIPTION_BEGIN(TType) ::Type* TType::GetStaticType() \
	{ \
		using ObjectType = TType; \
		ObjectType* __compiler__hack__ = nullptr; \
		(void)__compiler__hack__; \
		static Type* type = nullptr; \
		if (type != nullptr) \
			return type; \
		type = new ::Type(); \
		type->Fields.Clear(); \
		type->Abstract = true;

#define RAYOBJECT_DESCRIPTION_CREATEABLE() type->Abstract = false; \
		type->CreateFn = []() -> RayObject* { return new ObjectType(); }; 
			
#define RAYOBJECT_DESCRIPTION_NAME(name) type->Name = name;
#define RAYOBJECT_DESCRIPTION_FIELD(name, ttype, etype) type->Fields.PushBack(FieldInfo { etype, String(#name), offsetof(ObjectType, name), sizeof(ttype) });

#define RAYOBJECT_DESCRIPTION_END(TType) return type; \
	} \
	::Type* TType::GetType() \
	{ \
		return GetStaticType(); \
	} \
	struct AUTOREGISTER__##TType \
	{ \
		AUTOREGISTER__##TType() \
		{ \
			ObjectDb::__Internal_RegisterObjectStatic(TType::GetStaticType()); \
		} \
	}; \
	static AUTOREGISTER__##TType __AUTOREGISTER__##TType;
