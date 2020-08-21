#pragma once
#include <d3d12.h>

namespace ray::renderer_core_api
{

	class RootSignature
	{
	public:
		void Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);
		ID3D12RootSignature* GetRootSignature() const noexcept { return _rootSignature; }

	private:
		ID3D12RootSignature* _rootSignature;

	};

}
