#pragma once
#include <d3d12.h>

namespace ray::renderer_core_api
{

	class RootSignature
	{
	public:
		ID3D12RootSignature* GetRootSignature() const noexcept { return _rootSignature; }

	private:
		ID3D12RootSignature* _rootSignature;

	};

}
