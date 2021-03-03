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
protected:
	u64 _id = -1;
	
	virtual bool LoadFrom(IFile* path) = 0;
public:
	virtual ResourceType GetResourceType() const noexcept = 0;
	
	u64 GetId()
	{ return _id; }
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
};

class RAY_RESOURCES_API RLevel final : public IRResource
{
public:
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
protected:
	bool LoadFrom(IFile* path) override;
public:
	ResourceType GetResourceType() const noexcept override;
};
