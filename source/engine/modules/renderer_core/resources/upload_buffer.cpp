#include "upload_buffer.hpp"
#include <renderer_core/renderer.hpp>
#include <renderer_core/dx12_helper_functions.hpp>
#include "texture.hpp"
#include <core/math/common.hpp>

using namespace ray::core::math;

namespace ray::renderer_core_api::resources
{

	void UploadBuffer::Initialize(u64 size) noexcept
	{
		_maxPoolSize = size;

		D3D12_RESOURCE_DESC pool;
		pool.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		pool.Alignment = 0;
		pool.DepthOrArraySize = 1;
		pool.Flags = D3D12_RESOURCE_FLAG_NONE;
		pool.MipLevels = 1;
		pool.Format = DXGI_FORMAT_UNKNOWN;
		pool.SampleDesc.Count = 1;
		pool.SampleDesc.Quality = 0;
		pool.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		pool.Width = size;
		pool.Height = 1;
		
		auto heapProps = dx12::DescribeHeapProps(D3D12_HEAP_TYPE_UPLOAD);
		globals::gDevice->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &pool, D3D12_RESOURCE_STATE_COPY_SOURCE, nullptr, IID_PPV_ARGS(&_pool));

		D3D12_RANGE range;
		range.Begin = 0;
		range.End = 0;

		void* data;

		_pool->Map(0, &range, &data);
		_currentPointer = _begin = reinterpret_cast<u8*>(data);
		_end = _begin + size;
	}

	void UploadBuffer::Destroy() noexcept
	{
		if (_begin != nullptr)
		{
			_pool->Unmap(0, nullptr);
			_pool->Release();
			_begin = _end = _currentPointer = nullptr;
		}
	}

	u8* UploadBuffer::SetBufferData(void* buffer, size_t elementsCount, size_t elementSize) noexcept
	{
		return SetBufferDataToUploadBuffer(buffer, elementsCount * elementSize, sizeof(elementSize));
	}

	u8* UploadBuffer::SetConstantBufferData(void* buffer, size_t bufferSize) noexcept
	{
		return SetBufferDataToUploadBuffer(buffer, bufferSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
	}

	u8* UploadBuffer::SetTextureData(RTexture& texture) noexcept
	{
		size_t bitesPerPixel = BitesPerPixel(DXGI_FORMAT_R32G32B32A32_FLOAT);

		D3D12_SUBRESOURCE_FOOTPRINT footprint;
		footprint.Depth = 1;
		footprint.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		footprint.Height = texture.GetDimensions().y;
		footprint.Width = texture.GetDimensions().x;
		footprint.RowPitch = AlignUp(bitesPerPixel * footprint.Width, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		size_t textureSize = footprint.Height * footprint.RowPitch;
		_currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_currentPointer), D3D12_TEXTURE_DATA_PLACEMENT_ALIGNMENT));
		check(_currentPointer + textureSize <= _end)

		D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedFootprint;
		placedFootprint.Footprint = footprint;
		placedFootprint.Offset = _currentPointer - _begin;

		for (size_t i = 0; i < footprint.Height; ++i)
		{
			void* dest = _currentPointer + i * footprint.RowPitch;
			memcpy(dest, texture.GetData().GetData(), footprint.Width * bitesPerPixel);
		}

		u8* ret = _currentPointer;
		_currentPointer += textureSize;

		return ret;
	}

	u8* UploadBuffer::SetBufferDataToUploadBuffer(void* buffer, size_t bufferSize, size_t alignment) noexcept
	{
		_currentPointer = reinterpret_cast<u8*>(AlignUp(reinterpret_cast<size_t>(_currentPointer), alignment));
		check(_currentPointer + bufferSize <= _end)
		memcpy(_currentPointer, buffer, bufferSize);

		u8* ret = _currentPointer;
		_currentPointer += bufferSize;

		return ret;
	}

}