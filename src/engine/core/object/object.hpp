#pragma once
#include <ray/type/extensions.hpp>
#include "core/serialization/Serializer.hpp"

namespace ray::core
{

/**
 *  Defines an engine object that can be deserialized, serailized and reflected.
 */
struct Object
{
    virtual string GetClassName() { return ""; }

    virtual void OnSerialize(Serializer* serializer) { }

    virtual void OnDeserialize(Serializer* serializer) { }

    virtual ~Object() = default;
};

}
