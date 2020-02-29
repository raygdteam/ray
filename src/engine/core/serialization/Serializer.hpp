#pragma once
#include "core/pch.hpp"
#include <string>
#include "stream.hpp"
#include <memory>


namespace ray
{

class Object;

class Serializer
{
public:
    enum class Format
    {
        eBinary,
        eJson
    };

    virtual ~Serializer();

    static Serializer* CreateReader(const string& path);

    string GetPath() const;

    virtual void Close() = 0;

    virtual Format GetFormat() const = 0;

    virtual u32 GetVersionMajor() const;

    virtual u32 GetVersionMinor() const;

    virtual void WriteBool(bool value, bool defaultValue) = 0;

    virtual void WriteU8(u8 value, int defaultValue) = 0;
	virtual void WriteU16(u16 value, int defaultValue) = 0;
    virtual void WriteU32(u32 value, int defaultValue) = 0;
    virtual void WriteU64(u64 value, int defaultValue) = 0;

    virtual void WriteFloat(float value, float defaultValue) = 0;

    virtual void WriteString(const char* value, const char* defaultValue) = 0;

    virtual void WriteStringList(size_t count) = 0;

    virtual void WriteObject(std::shared_ptr<Object> value) = 0;

    virtual void WriteObjectList(size_t count) = 0;

    virtual void WriteIntArray(const int* data, size_t count) = 0;

    virtual void WriteFloatArray(const float* data, size_t count) = 0;

    virtual void WriteByteArray(const unsigned char* data, size_t count) = 0;

    virtual int ReadEnum(const char* enumName, int defaultValue) = 0;

    virtual bool ReadBool(bool defaultValue) = 0;


    virtual void ReadU8(u8 value, int defaultValue) = 0;
    virtual void ReadU16(u16 value, int defaultValue) = 0;
    virtual void ReadU32(u32 value, int defaultValue) = 0;
    virtual void ReadU64(u64 value, int defaultValue) = 0;

    virtual float ReadFloat(float defaultValue) = 0;

    virtual void ReadString(string& value, const char* defaultValue) = 0;

    virtual size_t ReadStringList(const char* propertyName) = 0;

    virtual std::shared_ptr<Object> readObject() = 0;

    virtual size_t ReadObjectList() = 0;

    virtual size_t ReadIntArray(int** data) = 0;

    virtual size_t ReadFloatArray(float** data) = 0;

    virtual size_t ReadByteArray(unsigned char** data) = 0;

protected:

    enum class Type
    {
        eReader,
        eWriter
    };

    Serializer(Type type, const string& path, quantum::Stream* stream, uint32_t versionMajor, uint32_t versionMinor);

    Type _type;
    string _path;
	quantum::Stream* _stream;
    int _version[2];

};


}
