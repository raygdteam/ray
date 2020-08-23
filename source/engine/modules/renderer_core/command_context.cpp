#include "command_context.hpp"
#include "renderer.hpp"
#include "d3dx12.h"
#include <core/extended_instuctions/sse/common.hpp>
#include <core/math/common.hpp>
#include <core/debug/assert.hpp>

namespace sse = ray::core::sse;
namespace math = ray::core::math;

#pragma clang diagnostic ignored "-Wmissing-braces" 

namespace ray::renderer_core_api
{
	// ------------------------ CONTEXT MANAGER ------------------------ //

	ray::CriticalSection ContextManager::_sContextManagerMutex;
	std::vector<CommandContext*> ContextManager::_sContextPool[4];
	std::queue<CommandContext*> ContextManager::_sAvailableContexts[4];

	CommandContext* ContextManager::AllocateContext(D3D12_COMMAND_LIST_TYPE type) noexcept
	{
		_sContextManagerMutex.Enter();

		auto& availableContexts = _sAvailableContexts[type];

		CommandContext* ret = nullptr;

		if (availableContexts.empty())
		{
			ret = new CommandContext(type);
			ret->Initialize();
			_sContextPool[static_cast<u32>(type)].push_back(ret);
		}
		else
		{
			ret = availableContexts.front();
			ret->Reset();
			availableContexts.pop();
		}

		check(ret != nullptr);
		check(ret->_type == type);

		_sContextManagerMutex.Leave();
		return ret;
	}

	void ContextManager::FreeContext(CommandContext* context) noexcept
	{
		_sContextManagerMutex.Enter();

		check(context != nullptr);
		_sAvailableContexts[static_cast<u32>(context->_type)].push(context);

		_sContextManagerMutex.Leave();
	}

	void ContextManager::DestroyAllContexts() noexcept
	{
		for (u16 i = 0; i < 4; i++)
			_sContextPool[i].clear();
	}

	// ------------------------ COMMAND CONTEXT ------------------------ //

	CommandContext& CommandContext::Begin()
	{
		CommandContext* newContext = globals::gContextManager.AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
		return *newContext;
	}

	void CommandContext::DestroyAllContexts()
	{
		LinearAllocator::DestroyAll();

		globals::gContextManager.DestroyAllContexts();
	}

	u64 CommandContext::Flush(bool bWaitForComplition)
	{
		FlushResourceBarriers();

		check(_commandAllocator != nullptr);

		u64 fenceValue = globals::gCommandListManager.GetQueue(_type).ExecuteCommandList(_commandList);

		if (bWaitForComplition)
			globals::gCommandListManager.WaitForFence(fenceValue);

		_commandList->Reset(_commandAllocator, nullptr);

		if (_graphicsRootSig)
			_commandList->SetGraphicsRootSignature(_graphicsRootSig);
		if (_computeRootSig)
			_commandList->SetComputeRootSignature(_computeRootSig);
		if (_currentPipelineState)
			_commandList->SetPipelineState(_currentPipelineState);

		// TODO: Set Pipeline state

		return fenceValue;
	}

	u64 CommandContext::Finish(bool bWaitForComplition)
	{
		FlushResourceBarriers();

		check(_commandAllocator != nullptr);

		CommandQueue& queue = globals::gCommandListManager.GetQueue(_type);
		u64 fenceValue = queue.ExecuteCommandList(_commandList);
		queue.DiscardAllocator(fenceValue, _commandAllocator);
		_commandAllocator = nullptr;

		_cpuLinearAllocator.CleanupUsedPages(fenceValue);
		_gpuLinearAllocator.CleanupUsedPages(fenceValue);
		// TODO: 

		if (bWaitForComplition)
			globals::gCommandListManager.WaitForFence(fenceValue);

		globals::gContextManager.FreeContext(this);

		return fenceValue;
	}

	CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type)
		: _type(type)
		, _commandList(nullptr)
		, _commandAllocator(nullptr)
		, _currentPipelineState(nullptr)
		, _graphicsRootSig(nullptr)
		, _computeRootSig(nullptr)
		, _cpuLinearAllocator(LinearAllocatorType::eCpuWritable)
		, _gpuLinearAllocator(LinearAllocatorType::eGpuExclusive)
		, _numBarriersToFlush(0)

		//TODO: 
	{}

	CommandContext::~CommandContext()
	{
		if (_commandList != nullptr)
		{
			_commandList->Release();
			_commandList = nullptr;
		}

		if (_commandAllocator != nullptr)
		{
			_commandAllocator->Release();
			_commandAllocator = nullptr;
		}
	}
	
	void CommandContext::Initialize()
	{
		globals::gCommandListManager.CreateNewCommandList(_type, &_commandAllocator, &_commandList);
	}

	void CommandContext::Reset()
	{
		check(_commandList != nullptr && _commandAllocator == nullptr);
		_commandAllocator = globals::gCommandListManager.GetQueue(_type).RequestAllocator();
		_commandList->Reset(_commandAllocator, nullptr);
		auto hr = globals::gDevice->GetDeviceRemovedReason();
		if(hr == S_OK) {}
		//TODO:
	}

	void CommandContext::InitializeTexture(resources::GpuResource& dest, u32 numSubResources, const void* data, u64 rowPitch, u64 slicePitch)
	{
		u64 uploadBufferSize = GetRequiredIntermediateSize(dest.GetResource(), 0, numSubResources);
		
		CommandContext& context = CommandContext::Begin();
		DynAlloc uploadBuffer = context.ReserveUploadMemory(uploadBufferSize);
		
		D3D12_SUBRESOURCE_DATA srcData;
		srcData.pData = data;
		srcData.RowPitch = rowPitch;
		srcData.SlicePitch = slicePitch;
		
		UpdateSubresources(context._commandList, dest.GetResource(), uploadBuffer.Buffer.GetResource(), 0, 0, numSubResources, &srcData);
		context.TransitionResource(dest, D3D12_RESOURCE_STATE_GENERIC_READ);

		context.Finish(true);
	}

	void CommandContext::InitializeTextureArraySlice(resources::GpuResource& dest, u64 sliceIndex, resources::GpuResource& src)
	{
		CommandContext& context = CommandContext::Begin();

		context.TransitionResource(dest, D3D12_RESOURCE_STATE_COPY_DEST);
		context.FlushResourceBarriers();

		const D3D12_RESOURCE_DESC& destDesc = dest.GetResource()->GetDesc();
		const D3D12_RESOURCE_DESC& srcDesc = src.GetResource()->GetDesc();
		(void)srcDesc;

		check(sliceIndex < destDesc.DepthOrArraySize&&
			srcDesc.DepthOrArraySize == 1 &&
			destDesc.Width == srcDesc.Width &&
			destDesc.Height == srcDesc.Height &&
			destDesc.MipLevels <= srcDesc.MipLevels
		);

		UINT subResourceIndex = sliceIndex * destDesc.MipLevels;

		for (UINT i = 0; i < destDesc.MipLevels; ++i)
		{
			D3D12_TEXTURE_COPY_LOCATION destCopyLocation =
			{
				dest.GetResource(),
				D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
				subResourceIndex + i
			};

			D3D12_TEXTURE_COPY_LOCATION srcCopyLocation =
			{
				src.GetResource(),
				D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
				i 
			};

			context._commandList->CopyTextureRegion(&destCopyLocation, 0, 0, 0, &srcCopyLocation, nullptr);
		}

		context.TransitionResource(dest, D3D12_RESOURCE_STATE_GENERIC_READ);
		context.Finish(true);
	}

	void CommandContext::ReadbackTexture2D(resources::GpuResource& readbackBuffer, resources::PixelBuffer& srcBuffer)
	{
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedSubresource;
		auto desc = srcBuffer.GetResource()->GetDesc();
		globals::gDevice->GetCopyableFootprints(&desc, 0, 1, 0, &placedSubresource, nullptr, nullptr, nullptr);

		CommandContext& context = CommandContext::Begin();
		context.TransitionResource(srcBuffer, D3D12_RESOURCE_STATE_COPY_SOURCE, true);

		CD3DX12_TEXTURE_COPY_LOCATION srcCopyLocation(srcBuffer.GetResource(), 0);
		CD3DX12_TEXTURE_COPY_LOCATION destCopyLocation(readbackBuffer.GetResource(), placedSubresource);

		context._commandList->CopyTextureRegion(&destCopyLocation, 0, 0, 0, &srcCopyLocation, nullptr);
		context.Finish(true);
	}


	void CommandContext::InitializeBuffer(resources::GpuResource& dest, const void* data, size_t numBytes, size_t offset)
	{
		CommandContext& context = CommandContext::Begin();
		DynAlloc mem = context.ReserveUploadMemory(numBytes);

		sse::MemCopy(mem.Data, data, math::DivideByMultiple(numBytes, 16));
		
		context.TransitionResource(dest, D3D12_RESOURCE_STATE_COPY_DEST, true);
		context._commandList->CopyBufferRegion(dest.GetResource(), offset, mem.Buffer.GetResource(), 0, numBytes);
		context.TransitionResource(dest, D3D12_RESOURCE_STATE_GENERIC_READ);

		context.Finish(true);
	}

	void CommandContext::WriteBuffer(resources::GpuResource& dest, size_t destOffset, const void* data, size_t numBytes)
	{
		check(data != nullptr && math::IsAligned(numBytes, 16));
		DynAlloc mem = _cpuLinearAllocator.Allocate(numBytes, 512);
		sse::MemCopy(mem.Data, data, math::DivideByMultiple(numBytes, 16));
		CopyBufferRegion(dest, destOffset, mem.Buffer, mem.Offset, numBytes);
	}

	void CommandContext::FillBuffer(resources::GpuResource& dest, size_t destOffset, float value, size_t numBytes)
	{
		DynAlloc mem = _cpuLinearAllocator.Allocate(numBytes, 512);
		__m128 vectorValue = _mm_set1_ps(value);
		sse::MemFill(mem.Data, vectorValue, math::DivideByMultiple(numBytes, 16));
		CopyBufferRegion(dest, destOffset, mem.Buffer, mem.Offset, numBytes);
	}

	void CommandContext::TransitionResource(resources::GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate)
	{
		D3D12_RESOURCE_STATES oldState = dest._usageState;
		if (newState != oldState)
		{
			check(_numBarriersToFlush < 16);
			D3D12_RESOURCE_BARRIER& barrier = _barriers[_numBarriersToFlush++];
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.StateAfter = newState;
			barrier.Transition.StateBefore = oldState;
			barrier.Transition.pResource = dest._resource;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

			if (newState == dest._transitioningState)
			{
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
				dest._transitioningState = static_cast<D3D12_RESOURCE_STATES>(-1);
			}
			else
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			dest._usageState = newState;
		}
		else if(newState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
		{ /*TODO:*/ }

		if (bFlushImmediate || _numBarriersToFlush == 16)
			FlushResourceBarriers();
	}

	void CommandContext::BeginResourceTransition(resources::GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate)
	{
		if (dest._transitioningState != static_cast<D3D12_RESOURCE_STATES>(-1))
			TransitionResource(dest, dest._transitioningState);

		auto oldState = dest._usageState;
		if (oldState != newState)
		{
			check(_numBarriersToFlush < 16);
			auto& barrier = _barriers[_numBarriersToFlush++];

			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = dest._resource;
			barrier.Transition.StateAfter = newState;
			barrier.Transition.StateBefore = oldState;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;

			dest._transitioningState = newState;
		}

		if (bFlushImmediate || _numBarriersToFlush == 16)
			FlushResourceBarriers();
	}

	inline void CommandContext::FlushResourceBarriers()
	{
		if (_numBarriersToFlush > 0)
		{
			_commandList->ResourceBarrier(_numBarriersToFlush, _barriers);
			_numBarriersToFlush = 0;
		}
	}

	void CommandContext::CopyBuffer(resources::GpuResource& dest, resources::GpuResource& src)
	{
		TransitionResource(dest, D3D12_RESOURCE_STATE_COPY_DEST);
		TransitionResource(src, D3D12_RESOURCE_STATE_COPY_SOURCE);
		FlushResourceBarriers();
		_commandList->CopyResource(dest.GetResource(), src.GetResource());
	}

	void CommandContext::CopyBufferRegion(resources::GpuResource& dest, size_t destOffset, resources::GpuResource& src, size_t srcOffset, size_t numBytes)
	{
		TransitionResource(dest, D3D12_RESOURCE_STATE_COPY_DEST);
		TransitionResource(src, D3D12_RESOURCE_STATE_COPY_SOURCE);
		FlushResourceBarriers();
		_commandList->CopyBufferRegion(dest.GetResource(), destOffset, src.GetResource(), srcOffset, numBytes);
	}

	void CommandContext::CopySubresource(resources::GpuResource& dest, u32 destSubIndex, resources::GpuResource& src, u32 srcSubIndex)
	{
		FlushResourceBarriers();

		D3D12_TEXTURE_COPY_LOCATION destLocation =
		{
			dest.GetResource(),
			D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
			{ destSubIndex }
		};

		D3D12_TEXTURE_COPY_LOCATION srcLocation =
		{
			src.GetResource(),
			D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
			{ srcSubIndex }
		};

		_commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);
	}

	void CommandContext::BindDescriptorHeaps()
	{
		u32 nonNullHeaps = 0;
		ID3D12DescriptorHeap* heapsToBind[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
		for (size_t i = 0; i < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++i)
		{
			auto it = _currentDescriptorHeaps[i];
			if (it != nullptr)
				heapsToBind[nonNullHeaps++] = it;
		}

		if (nonNullHeaps > 0)
			_commandList->SetDescriptorHeaps(nonNullHeaps, heapsToBind);
	}

	// ------------------------ COMPUTE CONTEXT ------------------------ //

	ComputeContext& ComputeContext::Begin(bool bAsync)
	{
		ComputeContext& newContext = globals::gContextManager.AllocateContext
					(bAsync ? D3D12_COMMAND_LIST_TYPE_COMPUTE : D3D12_COMMAND_LIST_TYPE_DIRECT)->GetComputeContext();

		return newContext;
	}

	// ------------------------ GRAPHICS CONTEXT ------------------------ //

	void GraphicsContext::ClearColor(resources::ColorBuffer& target)
	{
		float color[4];
		target.GetClearColor(&color[0], &color[1], &color[2], &color[3]);
		_commandList->ClearRenderTargetView(target.GetRTV(), color, 0, nullptr);
	}

	void GraphicsContext::SetRenderTargets(u32 numRTV, D3D12_CPU_DESCRIPTOR_HANDLE* rtv)
	{
		_commandList->OMSetRenderTargets(numRTV, rtv, static_cast<BOOL>(false), nullptr);
	}

	void GraphicsContext::SetRenderTargets(u32 numRTV, D3D12_CPU_DESCRIPTOR_HANDLE* rtv, D3D12_CPU_DESCRIPTOR_HANDLE dsv)
	{
		_commandList->OMSetRenderTargets(numRTV, rtv, static_cast<BOOL>(false), &dsv);
	}

	void GraphicsContext::SetViewport(const D3D12_VIEWPORT& viewport)
	{
		_commandList->RSSetViewports(1, &viewport);
	}

	void GraphicsContext::SetViewport(float x, float y, float w, float h, float minDepth, float maxDepth)
	{
		D3D12_VIEWPORT viewport;
		viewport.Width = w;
		viewport.Height = h;
		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		viewport.MaxDepth = maxDepth;
		viewport.MinDepth = minDepth;

		_commandList->RSSetViewports(1, &viewport);
	}

	void GraphicsContext::SetScissor(const D3D12_RECT& rect)
	{
		check(rect.left < rect.right && rect.top < rect.bottom);
		_commandList->RSSetScissorRects(1, &rect);
	}

	void GraphicsContext::SetScissor(u32 left, u32 top, u32 right, u32 bottom)
	{
		CD3DX12_RECT rect(left, top, right, bottom);
		SetScissor(rect);
	}

	void GraphicsContext::SetViewportAndScissor(const D3D12_VIEWPORT& viewport, const D3D12_RECT& rect)
	{
		check(rect.left < rect.right&& rect.top < rect.bottom);
		_commandList->RSSetViewports(1, &viewport);
		_commandList->RSSetScissorRects(1, &rect);
	}

	void GraphicsContext::SetViewportAndScissor(u32 x, u32 y, u32 w, u32 h)
	{
		SetViewport(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
		SetScissor(x, y, x + w, y + h);
	}

	void GraphicsContext::SetBlendFactor(float r, float g, float b, float a)
	{
		float color[4] =
		{
			r, g, b, a
		};
		_commandList->OMSetBlendFactor(color);
	}

	void GraphicsContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology)
	{
		_commandList->IASetPrimitiveTopology(primitiveTopology);
	}

	void GraphicsContext::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW& ibView)
	{
		_commandList->IASetIndexBuffer(&ibView);
	}

	void GraphicsContext::SetVertexBuffers(u32 startSlot, u32 count, const D3D12_VERTEX_BUFFER_VIEW* vbViews)
	{
		_commandList->IASetVertexBuffers(startSlot, count, vbViews);
	}

	void GraphicsContext::SetDynamicVB(u32 startSlot, size_t numVertices, size_t vertexStride, const void* data)
	{
		check(data != nullptr && math::IsAligned(data, 16))
		size_t bufferSize = math::AlignUp(numVertices * vertexStride, 16);

		auto vb = _cpuLinearAllocator.Allocate(bufferSize);
		sse::MemCopy(vb.Data, data, bufferSize >> 4);

		D3D12_VERTEX_BUFFER_VIEW view;
		view.BufferLocation = vb.GpuVirtualAddress;
		view.SizeInBytes = static_cast<u32>(bufferSize);
		view.StrideInBytes = static_cast<u32>(vertexStride);

		_commandList->IASetVertexBuffers(startSlot, 1, &view);
	}

	void GraphicsContext::SetDynamicIB(size_t indexCount, const u32* data, bool b32Bit)
	{
		check(data != nullptr && math::IsAligned(data, 16))
		
		size_t bufferSize = math::AlignUp(indexCount * (b32Bit ? sizeof(u32) : sizeof(u16)), 16);
		auto ib = _cpuLinearAllocator.Allocate(bufferSize);
		sse::MemCopy(ib.Data, data, bufferSize >> 4);

		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = ib.GpuVirtualAddress;
		view.Format = b32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = bufferSize;

		_commandList->IASetIndexBuffer(&view);
	}

	void GraphicsContext::Draw(u32 vertexCount, u32 vertexStartOffset)
	{
		DrawInstanced(vertexCount, 1, vertexStartOffset);
	}

	void GraphicsContext::DrawIndexed(u32 indexCount, u32 startIndexLocation, s32 baseVertexLocation)
	{
		DrawIndexedInstanced(indexCount, 1, startIndexLocation, baseVertexLocation, 0);
	}

	void GraphicsContext::DrawInstanced(u32 vertexCountPerInstance, u32 instanceCount, u32 startVertexLocation, u32 startInstanceLocation)
	{
		FlushResourceBarriers();
		// TODO: 
		_commandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
	}

	void GraphicsContext::DrawIndexedInstanced(u32 indexCountPerInstance, u32 instanceCount, u32 startIndexLocation, s32 baseVertexLocation, u32 startInstanceLocation)
	{
		FlushResourceBarriers();
		// TODO: 
		_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
	}



}
