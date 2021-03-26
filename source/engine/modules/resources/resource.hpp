#pragma once
#include <core/object/object.hpp>
#include <core/math/vector.hpp>
#include <core/file_system/file_system.hpp>

#include <engine/engine/engine_def.hpp>
#include <resources/resource_def.hpp>
#include <resources/resource_type.hpp>

class RAY_RESOURCES_API IRResource : public RayObject
{
	RAYOBJECT_BODY(IRResource, RayObject);

	friend class ResourceManager;

	String _name;
protected:
	u64 _id = -1;
	bool _loaded = false;
	
	virtual bool LoadFrom(IFile* path) = 0;
public:
	virtual ResourceType GetResourceType() const noexcept = 0;
	
	u64 GetId()
	{ return _id; }

	String& GetName()
	{
		return _name;
	}

	bool IsLoaded()
	{
		return _loaded;
	}
	
	virtual void Unload() = 0;
};

class RAY_RESOURCES_API RTexture final : public IRResource
{
	RAYOBJECT_BODY(RTexture, IRResource);
	
	Array<FVector4> _data;
	FVector2 _dimensions;
	
	friend class ResourceManager;

protected:
	RTexture();
	bool LoadFrom(IFile* path) override;

public:
	ResourceType GetResourceType() const noexcept override;
	
	const Array<FVector4>& GetData() const;
	const FVector2& GetDimensions() const;

	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;

	void Unload() override;
};

class RAY_RESOURCES_API RMaterial : public IRResource
{
	RAYOBJECT_BODY(RMaterial, IRResource);
	friend class ResourceManager;
public:
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
protected:
	RMaterial() {}
	bool LoadFrom(IFile* path) override;
public:
	ResourceType GetResourceType() const noexcept override;
	void Unload() override;

public:
	// Add properties here
};

class RAY_RESOURCES_API RMaterialInstance : public IRResource
{
	RAYOBJECT_BODY(RMaterialInstance, IRResource);
	friend class ResourceManager;
public:
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
protected:
	RMaterialInstance() {}
	bool LoadFrom(IFile* path) override;
public:
	ResourceType GetResourceType() const noexcept override;
	void Unload() override;

public:
	// Add properties here
	String Texture;
};

class RConfiguration : public IRResource
{
	RAYOBJECT_BODY(RConfiguration, IRResource);
	friend class ResourceManager;
public:
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
protected:
	RConfiguration() {}
	bool LoadFrom(IFile* path) override;
public:
	ResourceType GetResourceType() const noexcept override;
	void Unload() override;

	JsonValue Root;
};
