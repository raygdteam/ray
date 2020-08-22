#include "root_signature.hpp"
#include "renderer.hpp"
#include "d3dx12.h"
#include <core/debug/assert.hpp>

namespace ray::renderer_core_api
{
	void RootSignature::Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags)
	{
		// temp solution

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, flags);

		ID3DBlob* signature;
		auto hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
		check(hr == S_OK)

		hr = globals::gDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
		check(hr == S_OK)
	}
}