#pragma once
#include "../ray_renderer_core_base.hpp"
#include "../renderer_globals.hpp"
#include <core/math/vector.hpp>

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
		eVertexAndConstantBuffer,
		eCommon,
		eUnorderedAccess,
		eGenericRead
	};

	enum class ShaderType
	{
		eUnknown,

		eTypeless4,
		eFloat4,
		eInt4,
		eUint4,

		eTypeless3,
		eFloat3,
		eInt3,
		eUint3
	};

	struct SampleDesc
	{
		u32 Quality;
		u32 Count;
	};

	enum class TextureLayout
	{
		eUnknown,
		eRowMajor
	};

	enum class ResourceFlags
	{
		eNone = 0,
		eAllowRenderTarget = 0x1,
		eAllowDepthStencil = 0x2,
		eAllowUnordererAccess = 0x4,
		eDenyShaderResource = 0x8,
	};

	struct ResourceDesc
	{
		resources::ResourceType ResourceType;
		resources::ShaderType ShaderType;
		u64 Width;
		u64 Height;
		u64 Alignment;
		u16 DepthOrArraySize;
		u16 MipLevels;
		resources::SampleDesc SampleDesc;
		resources::TextureLayout TextureLayout;
		ResourceFlags Flags;
	};

	struct ResourceHeapProperties
	{
		ResourceUsage Usage;
	};

	struct Range
	{
		u32 Start;
		u32 End;
	};

	class IResource : public ray::renderer_core_api::IRRCBase
	{
	public:
		virtual ~IResource() {}

		virtual bool Map(u32 subresourceIndex, Range* range, void** data) = 0;
		virtual void Unmap(u32 subresourceIndex, Range* range) = 0;

		virtual GpuVirtualAddress GetGpuVirtualAddress() = 0;

		void SetData(void* data) noexcept { _data = data; }
		void SetSize(size_t sizeInBytes) noexcept { _size = sizeInBytes; }
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