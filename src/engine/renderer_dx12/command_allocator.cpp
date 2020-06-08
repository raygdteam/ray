#include "command_allocator.hpp"

namespace ray::renderer::d3d12
{
	bool D3D12CommandAllocator::Reset()
	{
		auto temp = static_cast<ID3D12CommandAllocator*>(GetInstance());
		auto hResult = temp->Reset();
		if (FAILED(hResult))
			return false;

		return true;
	}
}