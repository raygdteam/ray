#pragma once

#include <d3d12.h>
#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <renderer_core/command_context.hpp>
#include <renderer_core/resources/texture.hpp>
#include <renderer_core/descriptor_heap.hpp>
#include <renderer_core/resources/linear_allocator.hpp>

#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

namespace ray::renderer_core_api
{
	class Renderer2D;

	struct ManagedTexture
	{
		D3D12_CPU_DESCRIPTOR_HANDLE Descriptor;
		ID3D12Resource* Resource;
	};

	struct RAY_RENDERERCORE_API TextureManager
	{
		friend class Renderer2D;
	public:
		TextureManager();
		
		void PrepareTextures(CommandContext& ctx, RTexture** textures, size_t numTextures, bool bFlush = false);
		void LoadToGPU(CommandContext& ctx, RTexture** textures, size_t numTextures);
		static void Destroy() noexcept;

	private:
		static DynAlloc* _uploadedResources[64]; // temporary
		static ManagedTexture _gpuResources[64]; // temporary
		static UserDescriptorHeap _descriptorHeap;

	};
}