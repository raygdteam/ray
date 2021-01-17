#include <engine/world/components/transform.hpp>
#include "core/extended_instuctions/sse/common.hpp"

void Transform::Serialize(Archive& ar)
{
	//ar.Write<u32>(Crc32((u8*)GetType()->Name, strlen(GetType()->Name)));
	ar.Write(this->Position);
}

void Transform::Deserialize(Archive& ar)
{
	ar.Read(this->Position);
}

RAYOBJECT_DESCRIPTION_BEGIN(Transform)
	RAYOBJECT_DESCRIPTION_NAME("engine://world/components/Transform")
	RAYOBJECT_DESCRIPTION_CREATEABLE()
	RAYOBJECT_DESCRIPTION_FIELD(Position, FVector<2>, eVector2);
RAYOBJECT_DESCRIPTION_END(Transform)
