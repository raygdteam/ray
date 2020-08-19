#include <engine/world/components/transform.hpp>

void Transform::Serialize(Archive& ar)
{
	ar.Write(this->Position);
}

void Transform::Deserialize(Archive& ar)
{
	ar.Read(this->Position);
}
