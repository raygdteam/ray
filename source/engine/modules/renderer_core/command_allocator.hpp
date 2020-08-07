#pragma once
#include "ray_renderer_core_base.hpp"

/*
**	Vulkan:			VkCommandPool
**	Direct3D 12:	ID3D12CommandAllocator
*/

#include <core/threading/critical_section.hpp>
#include <vector>
#include <queue>

namespace ray::renderer_core_api
{
class ICommandAllocator : public IRRCBase
{
public:
	virtual ~ICommandAllocator() {};

	virtual bool Reset() = 0;
	
};

class CommandAllocatorPool
{
public:
	CommandAllocatorPool(CommandListType type) : _type(type) {}
	~CommandAllocatorPool();

	void Shutdown();

	size_t Size() { return _allocatorPool.size(); }

	ICommandAllocator* RequestAllocator(u64 completedFenceValue);
	void DiscardAllocator(ICommandAllocator* allocator, u64 fenceValue);

private:
	CommandListType _type;
	ray::CriticalSection _allocatorMutex;
	std::vector<ICommandAllocator*> _allocatorPool;
	std::queue<std::pair<u64, ICommandAllocator*>> _readyAllocators;

};

}