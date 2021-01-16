#include "command_context.hpp"
#include "renderer.hpp"
#include "resources/upload_buffer.hpp"
#include "resources/ring_buffer.hpp"
#include "resources/gpu_texture.hpp"
#include "d3dx12.h"
#include <core/extended_instuctions/sse/common.hpp>
#include <core/math/common.hpp>
#include <core/debug/assert.hpp>

#pragma clang diagnostic ignored "-Wmissing-braces" 

// ------------------------ CONTEXT MANAGER ------------------------ //

CriticalSection ContextManager::_sContextManagerMutex;
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
	CommandContext* newContext = gContextManager.AllocateContext(D3D12_COMMAND_LIST_TYPE_DIRECT);
	return *newContext;
}

void CommandContext::DestroyAllContexts()
{
	gContextManager.DestroyAllContexts();
}

u64 CommandContext::Flush(bool bWaitForComplition)
{
	FlushResourceBarriers();

	check(_commandAllocator != nullptr);

	u64 fenceValue = gCommandListManager.GetQueue(_type).ExecuteCommandList(_commandList);

	if (bWaitForComplition)
		gCommandListManager.WaitForFence(fenceValue);

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

	CommandQueue& queue = gCommandListManager.GetQueue(_type);
	u64 fenceValue = queue.ExecuteCommandList(_commandList);
	queue.DiscardAllocator(fenceValue, _commandAllocator);
	_commandAllocator = nullptr;
	// TODO: 

	if (bWaitForComplition)
		gCommandListManager.WaitForFence(fenceValue);

	gContextManager.FreeContext(this);

	return fenceValue;
}

CommandContext::CommandContext(D3D12_COMMAND_LIST_TYPE type)
	: _type(type)
	, _commandList(nullptr)
	, _commandAllocator(nullptr)
	, _currentPipelineState(nullptr)
	, _graphicsRootSig(nullptr)
	, _computeRootSig(nullptr)
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
	gCommandListManager.CreateNewCommandList(_type, &_commandAllocator, &_commandList);
}

void CommandContext::Reset()
{
	check(_commandList != nullptr && _commandAllocator == nullptr);
	_commandAllocator = gCommandListManager.GetQueue(_type).RequestAllocator();
	_commandList->Reset(_commandAllocator, nullptr);
	auto hr = gDevice->GetDeviceRemovedReason();
	if (hr == S_OK) {}
	//TODO:
}

void CommandContext::InitializeTexture(GpuResource& dest, UploadBuffer& src)
{
	CommandContext& context = CommandContext::Begin();
	auto textureDesc = dest.GetDesc();
	size_t bitesPerPixel = BytesPerPixel(textureDesc.Format);

	D3D12_SUBRESOURCE_FOOTPRINT footprint;
	footprint.Depth = 1;
	footprint.Format = textureDesc.Format;
	footprint.Height = textureDesc.Height;
	footprint.Width = textureDesc.Width;
	footprint.RowPitch = AlignUp(bitesPerPixel * footprint.Width, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedFootprint;
	placedFootprint.Footprint = footprint;
	placedFootprint.Offset = reinterpret_cast<const u8*>(textureDesc.UploadBufferData) - src.GetBeginPointer();

	context.CopyTextureRegion(dest, src, placedFootprint);
	context.TransitionResource(dest, D3D12_RESOURCE_STATE_GENERIC_READ);

	context.Finish(true);
}

void CommandContext::InitializeTextureArraySlice(GpuResource& dest, u64 sliceIndex, GpuResource& src)
{
	// TODO: 
	// still not supported

	/*CommandContext& context = CommandContext::Begin();

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
	context.Finish(true);*/
}

void CommandContext::ReadbackTexture2D(GpuResource& readbackBuffer, PixelBuffer& srcBuffer)
{
	// TODO:
	// still not supported

	/*D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedSubresource;
	auto desc = srcBuffer.GetResource()->GetDesc();
	globals::gDevice->GetCopyableFootprints(&desc, 0, 1, 0, &placedSubresource, nullptr, nullptr, nullptr);

	CommandContext& context = CommandContext::Begin();
	context.TransitionResource(srcBuffer, D3D12_RESOURCE_STATE_COPY_SOURCE, true);

	CD3DX12_TEXTURE_COPY_LOCATION srcCopyLocation(srcBuffer.GetResource(), 0);
	CD3DX12_TEXTURE_COPY_LOCATION destCopyLocation(readbackBuffer.GetResource(), placedSubresource);

	context._commandList->CopyTextureRegion(&destCopyLocation, 0, 0, 0, &srcCopyLocation, nullptr);
	context.Finish(true);*/
}


void CommandContext::InitializeBuffer(GpuResource& dest, UploadBuffer& src)
{
	auto& context = CommandContext::Begin();
	auto bufferDesc = dest.GetDesc();
	size_t srcOffset = reinterpret_cast<const u8*>(bufferDesc.UploadBufferData) - src.GetBeginPointer();

	context.CopyBufferRegion(dest, src, srcOffset, bufferDesc.SizeInBytes);
	context.TransitionResource(dest, D3D12_RESOURCE_STATE_GENERIC_READ);

	context.Finish(true);
}

void CommandContext::WriteBuffer(GpuResource& dest, size_t destOffset, const void* data, size_t numBytes)
{
	// TODO
	// we need it?

	/*check(data != nullptr && IsAligned(numBytes, 16));
	DynAlloc mem = _cpuLinearAllocator.Allocate(numBytes, 512);
	MemCopy(mem.Data, data, DivideByMultiple(numBytes, 16));
	CopyBufferRegion(dest, destOffset, mem.Buffer, mem.Offset, numBytes);*/
}

void CommandContext::FillBuffer(GpuResource& dest, size_t destOffset, float value, size_t numBytes)
{
	// TODO
	// we need it?

	/*
	DynAlloc mem = _cpuLinearAllocator.Allocate(numBytes, 512);
	__m128 vectorValue = _mm_set1_ps(value);
	MemFill(mem.Data, vectorValue, DivideByMultiple(numBytes, 16));
	CopyBufferRegion(dest, destOffset, mem.Buffer, mem.Offset, numBytes);*/
}

void CommandContext::TransitionResource(GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate)
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
	else if (newState == D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		/*
			TODO:
			still not supported
		*/
	}

	if (bFlushImmediate || _numBarriersToFlush == 16)
		FlushResourceBarriers();
}

void CommandContext::BeginResourceTransition(GpuResource& dest, D3D12_RESOURCE_STATES newState, bool bFlushImmediate)
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

void CommandContext::CopyBuffer(GpuResource& dest, GpuResource& src)
{
	TransitionResource(dest, D3D12_RESOURCE_STATE_COPY_DEST);
	TransitionResource(src, D3D12_RESOURCE_STATE_COPY_SOURCE);
	FlushResourceBarriers();
	_commandList->CopyResource(dest.GetNativeResource(), src.GetNativeResource());
}

void CommandContext::CopyBufferRegion(GpuResource& dest, UploadBuffer& src, size_t srcOffset, size_t numBytes)
{
	TransitionResource(dest, D3D12_RESOURCE_STATE_COPY_DEST, true);
	_commandList->CopyBufferRegion(dest.GetNativeResource(), 0, src.GetNativePool(), srcOffset, numBytes);
}

void CommandContext::CopyTextureRegion(GpuResource& dest, UploadBuffer& src, D3D12_PLACED_SUBRESOURCE_FOOTPRINT& srcFootprint)
{
	FlushResourceBarriers();

	D3D12_TEXTURE_COPY_LOCATION destLocation;
	destLocation.pResource = dest.GetNativeResource();
	destLocation.SubresourceIndex = 0;
	destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

	D3D12_TEXTURE_COPY_LOCATION srcLocation;
	srcLocation.pResource = src.GetNativePool();
	srcLocation.PlacedFootprint = srcFootprint;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;

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
	ComputeContext& newContext = gContextManager.AllocateContext
	(bAsync ? D3D12_COMMAND_LIST_TYPE_COMPUTE : D3D12_COMMAND_LIST_TYPE_DIRECT)->GetComputeContext();

	return newContext;
}

// ------------------------ GRAPHICS CONTEXT ------------------------ //

void GraphicsContext::ClearColor(ColorBuffer& target)
{
	float color[4];
	target.GetClearColor(&color[0], &color[1], &color[2], &color[3]);
	_commandList->ClearRenderTargetView(target.GetRTV(), color, 0, nullptr);
}

void GraphicsContext::ClearDepthAndStencil(DepthBuffer& target)
{
	_commandList->ClearDepthStencilView(target.GetDSV(), D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
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
	check(rect.left < rect.right&& rect.top < rect.bottom);
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

void GraphicsContext::SetDynamicVB(RingBuffer& ringBuffer, u32 startSlot, size_t numVertices, size_t vertexStride, const void* data)
{
	u8* vb = ringBuffer.SetBufferData(const_cast<void*>(data), numVertices, vertexStride);
	u64 offset = vb - ringBuffer.GetBeginPointer();

	D3D12_VERTEX_BUFFER_VIEW view;
	view.BufferLocation = ringBuffer.GetNativePool()->GetGPUVirtualAddress() + offset;
	view.SizeInBytes = static_cast<u32>(numVertices * vertexStride);
	view.StrideInBytes = static_cast<u32>(vertexStride);

	_commandList->IASetVertexBuffers(startSlot, 1, &view);
}

void GraphicsContext::SetDynamicIB(RingBuffer& ringBuffer, size_t indexCount, const u32* data, bool b32Bit)
{
	size_t elementSize = b32Bit ? 4 : 2;
	u8* ib = ringBuffer.SetBufferData(const_cast<u32*>(data), indexCount, elementSize);
	u64 offset = ib - ringBuffer.GetBeginPointer();

	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = ringBuffer.GetNativePool()->GetGPUVirtualAddress() + offset;
	view.Format = b32Bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
	view.SizeInBytes = elementSize * indexCount;

	_commandList->IASetIndexBuffer(&view);
}

void GraphicsContext::SetDynamicCBV(RingBuffer& ringBuffer, u32 rootIndex, size_t bufferSize, void* data)
{
	u8* cb = ringBuffer.SetConstantBufferData(const_cast<void*>(data), bufferSize);
	u64 offset = cb - ringBuffer.GetBeginPointer();
	u64 gpuVirtualAddress = ringBuffer.GetNativePool()->GetGPUVirtualAddress() + offset;

	_commandList->SetGraphicsRootConstantBufferView(rootIndex, gpuVirtualAddress);
}

void GraphicsContext::SetDescriptorTable(u32 rootIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
	_commandList->SetGraphicsRootDescriptorTable(rootIndex, handle);
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
	// still not supported
	_commandList->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}

void GraphicsContext::DrawIndexedInstanced(u32 indexCountPerInstance, u32 instanceCount, u32 startIndexLocation, s32 baseVertexLocation, u32 startInstanceLocation)
{
	FlushResourceBarriers();
	// TODO: 
	// still not supported
	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}


