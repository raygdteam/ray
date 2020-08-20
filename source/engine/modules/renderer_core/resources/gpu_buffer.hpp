#pragma once

#include "gpu_resource.hpp"

namespace ray::renderer_core_api
{
	class CommandContext;
}

namespace ray::renderer_core_api::resources
{
	class GpuBuffer : public GpuResource
	{
	public:
		virtual ~GpuBuffer() { Destroy(); }

		void Create(u32 numElements, u32 elementSize, const void* initialData = nullptr);
		void CreatePlaces(ID3D12Heap* backingHeap, size_t heapOffset, u32 numElements, u32 elementSize, const void* initialData = nullptr);

		D3D12_GPU_VIRTUAL_ADDRESS RootConstantBufferView() const noexcept { return _gpuVirtualAddress; }
		D3D12_CPU_DESCRIPTOR_HANDLE CreateConstantBufferView(size_t offset, size_t size);

		D3D12_VERTEX_BUFFER_VIEW VertexBufferView(size_t offset, size_t size, u32 stride);
		D3D12_VERTEX_BUFFER_VIEW VertexBufferView(u32 startIndex)
		{
			size_t offset = _elementSize * startIndex;
			return VertexBufferView(offset, (_bufferSize - offset), _elementSize);
		}

		D3D12_INDEX_BUFFER_VIEW IndexBufferView(size_t offset, size_t size, bool b32Bit = false);
		D3D12_INDEX_BUFFER_VIEW IndexBufferView(u32 startIndex)
		{
			size_t offset = _elementSize * startIndex;
			return IndexBufferView(offset, (_bufferSize - offset), _elementSize == 4);
		}

		size_t GetBufferSize() const noexcept { return _bufferSize; }
		u32 GetElementCount() const noexcept { return _elementCount; }
		u32 GetElementSize() const noexcept { return _elementSize; }
		const D3D12_CPU_DESCRIPTOR_HANDLE& GetSRV() const noexcept { return _srvHandle; }
		const D3D12_CPU_DESCRIPTOR_HANDLE& GetUAV() const noexcept { return _uavHandle; }

	protected:
		GpuBuffer()
			: _bufferSize(0)
			, _elementCount(0)
			, _elementSize(0)
		{
			_resourceFlags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
			_srvHandle.ptr = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1);
			_uavHandle.ptr = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(-1);
		}

		virtual void CreateDerivedViews() = 0;
		D3D12_RESOURCE_DESC DescribeBuffer() const noexcept;

		size_t _bufferSize;
		u32 _elementCount;
		u32 _elementSize;
		D3D12_RESOURCE_FLAGS _resourceFlags;

		D3D12_CPU_DESCRIPTOR_HANDLE _srvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE _uavHandle;

	};
}

