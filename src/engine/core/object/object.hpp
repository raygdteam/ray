#pragma once

namespace ray::core
{

/**
 *  Defines an engine object that can be deserialized, serailized and reflected.
 */
struct Object
{
    virtual std::string GetClassName() { return ""; }

    virtual void OnSerialize(Serializer* serializer) { }

    virtual void OnDeserialize(Serializer* serializer) { }

    virtual ~Object();
};

}
