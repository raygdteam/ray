#pragma once

#include <core/core.hpp>
#include <d3d12.h>

namespace ray::renderer_core_api::resources
{
	class GpuBuffer;
	class GpuTexture;

	// 
	class IResourceView
	{
	public:
		virtual void* GetNativeHandle() = 0;
	
	};

	class ShaderResourceView : public IResourceView
	{

	};

	class UnorderedAccessView : public IResourceView
	{

	};

	class DepthStencilView : public IResourceView
	{

	};

	class RenderTargetView : public IResourceView
	{

	};

	class VertexBufferView : public IResourceView
	{

	};

	class ConstantBufferView : public IResourceView
	{

	};

	class IndexBufferView : public IResourceView
	{

	};

}