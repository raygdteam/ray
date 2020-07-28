#include "gpu_resource.hpp"
#include <vector>
#include <queue>
#include <mutex>

#define DEFAULT_ALIGN 256

using namespace ray::renderer_core_api::resources;

namespace ray::renderer_core_api::memory
{
	struct DynAlloc
	{
		GpuResource& Buffer;
		size_t Size;
		size_t Offset;
		void* Data;
		GpuVirtualAddress GpuVirtualAddress;
	};

	class LinearAllocationPage : public GpuResource
	{
	public:
		LinearAllocationPage(IResource* inResource, ResourceState usage) : GpuResource()
		{
			_resource = inResource;
			_usageState = usage;
			_gpuVirtualAddress = _resource->GetGpuVirtualAddress();
			_resource->Map(0, nullptr, &_cpuVirtualAddress);
		}

		~LinearAllocationPage()
		{
			Unmap();
		}

		void Map()
		{
			if (_cpuVirtualAddress == nullptr)
				_resource->Map(0, nullptr, &_cpuVirtualAddress);
		}

		void Unmap()
		{
			if (_cpuVirtualAddress != nullptr)
			{
				_resource->Unmap(0, nullptr);
				_cpuVirtualAddress = nullptr;
			}
		}

	private:
		void* _cpuVirtualAddress;

	};

	enum LinearAllocatorType
	{
		eInvalidAllocator = -1,
		eGpuExclusive = 0, // resources::ResourceUsage::eDefault
		eCpuWritable = 1,   // resources::ResourceUsage::eUpload
		eNumAllocatorTypes = 2
	};

	const u32 gGpuAllocatorPageSize = 0x10000;    // 64K
	const u32 gCpuAllocatorPageSize = 0x200000;    // 2MB

	class LinearAllocatorPageManager
	{
	public:
		LinearAllocatorPageManager();

		LinearAllocationPage* RequestPage();
		LinearAllocationPage* CreateNewPage(size_t pageSize = 0);

		// Discarded pages will get recycled.  This is for fixed size pages.
		void DiscardPages(u64 fenceValue, const std::vector<LinearAllocationPage*>& pages);

		/* Freed pages will be destroyed once their fence has passed.  This is for single-use,
		"large" pages. */
		void FreeLargePages(u64 fenceValue, const std::vector<LinearAllocationPage*>& pages);

		void Destroy() { _pagePool.clear(); }

	private:
		static LinearAllocatorType _sDefaultType;

		LinearAllocatorType _type;
		std::vector<LinearAllocationPage*> _pagePool;
		std::queue<std::pair<u64, LinearAllocationPage*>> _retiredPage;
		std::queue<std::pair<u64, LinearAllocationPage*>> _deletionQueue;
		std::queue<LinearAllocationPage*> _availablePages;

		std::mutex _mutex;

	};

	class LinearAllocator
	{
	public:
		LinearAllocator(LinearAllocatorType type) 
			: _type(type)
			, _currentOffset(~static_cast<size_t>(0))
			, _currentPage(nullptr)
			, _pageSize(0)
		{
			if (!(type > eInvalidAllocator && type < eNumAllocatorTypes))
				return;

			_pageSize = (type == eGpuExclusive) ? gGpuAllocatorPageSize : gCpuAllocatorPageSize;
		}

		DynAlloc Allocate(size_t size, size_t align = DEFAULT_ALIGN);
		void CleanupUsedPages(u64 fenceValue);
		static void DestroyAll()
		{
			_sPageManager[0].Destroy();
			_sPageManager[1].Destroy();
		}

	private:
		static LinearAllocatorPageManager _sPageManager[eNumAllocatorTypes];

		LinearAllocatorType _type;
		size_t _currentOffset;
		size_t _pageSize;
		LinearAllocationPage* _currentPage;
		std::vector<LinearAllocationPage*> _retiredPages;
		std::vector<LinearAllocationPage*> _largePages;
		
	private:
		DynAlloc AllocateLargePage(size_t size);

	};
}