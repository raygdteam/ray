#pragma once

#include <d3d12.h>
#include <core/core.hpp>
#include <core/debug/assert.hpp>
#include <resources/resource_manager.hpp>
#include <renderer_core/command_context.hpp>
#include <renderer_core/resources/texture.hpp>
#include <renderer_core/descriptor_heap.hpp>
#include <renderer_core/resources/linear_allocator.hpp>

namespace ray::renderer_core_api
{
	struct ManagedTexture
	{
		D3D12_CPU_DESCRIPTOR_HANDLE Descriptor;
		ID3D12Resource* Resource;
	};

	class TextureManager
	{
	public:

		void PrepareTextures(CommandContext& ctx, RTexture* textures, size_t numTextures, bool bFlush = false);
		void LoadToGPU(CommandContext& ctx, RTexture* textures, size_t numTextures);

	private:
		DynAlloc _uploadedResources[64]; // temporary
		ManagedTexture _gpuResources[64]; // temporary
		UserDescriptorHeap _descriptorHeap;

	};
}