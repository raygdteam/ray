#pragma once
#include "../ray_renderer_core_base.hpp"

namespace ray::renderer_core_api::resources
{

enum class ResourceUsage
{
	eDefault, // cpu: haven't access; gpu: read/write
	eUpload,  // cpu: write; gpu: read
	eReadback // cpu: read; gpu: write
};

enum class ResourceType
{
	eBuffer,
	eTexture1D,
	eTexture2D,
	eTexture3D
};

enum class ResourceState
{
	ePresent,
	eCopyDest,
	eRenderTarget,
	eVertexAndConstantBuffer
};

enum class ShaderType
{
	eTypeless4,
	eFloat4,
	eInt4,
	eUint4,

	eTypeless3,
	eFloat3,
	eInt3,
	eUint3
};

struct ResourceDesc
{
	ResourceType _resource_type;
	ShaderType _shader_type;
	u64 _width;
	u64 _height;
	u64 _alignment;
	//other properties for textures...
};
	
class IResource : public ray::renderer_core_api::IRRCBase
{
public:
	virtual ~IResource() {}
	

	void SetData(void* data, size_t size /*in bytes*/) noexcept { _data = data; }
	void* GetData() const noexcept { return _data; }
	size_t GetSize() const noexcept { return _size; }

protected:
	void* _data;
	size_t _size;

};


class IResourceBarrier : public ray::renderer_core_api::IRRCBase
{
public:
	virtual ~IResourceBarrier() {}
	/**
	 *  Transitions between resource states
	 *  @param 
	 *	@param 
	 *	@param 
	 */
	virtual void Transition(IResource* inResource, ResourceState beforeState, ResourceState afterState) = 0;
};

}