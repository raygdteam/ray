#pragma once
#include <ray/type/extensions.hpp>

namespace ray
{

/**
 *  Defines an engine object that can be deserialized, serailized and reflected.
 */
struct Object
{
    virtual string GetClassName() { return ""; }

    virtual void OnSerialize() { }

    virtual void OnDeserialize() { }

    virtual ~Object() = default;
};

}
