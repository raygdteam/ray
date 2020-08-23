#include "root_signature.hpp"
#include "renderer.hpp"
#include "d3dx12.h"
#include <core/debug/assert.hpp>

namespace ray::renderer_core_api
{
	RootSignatureParameter::RootSignatureParameter()
	{
		_parameter = {};
	}

	void RootSignature::Begin(u32 numParams, u32 numStaticSamplers)
	{
		check(_rootParameters.IsEmpty());

		for (u32 i = 0; i < numParams; ++i)
		{
			_rootParameters.PushBack(new RootSignatureParameter());
		}
	}

	void RootSignature::Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags)
	{
		/*
		 * It is safe to reinterpret_cast<> since that class has no vtable, nor has any virtual functions.
		 * The D3D12_ROOT_PARAMETER comes first in class structure.
		 */
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.NumParameters = u32(_rootParameters.Size());
		rootSignatureDesc.pParameters = reinterpret_cast<D3D12_ROOT_PARAMETER*>(_rootParameters.data());
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = flags;

		ID3DBlob* signature = nullptr;
		auto hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
		check(hr == S_OK)

		hr = globals::gDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_rootSignature));
		check(hr == S_OK)
	}
}