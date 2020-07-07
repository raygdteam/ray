#include "pch.hpp"
#include "d3d12_pixel_shader.hpp"

namespace ray::renderer::d3d12
{
	bool D3D12PixelShader::Compile(pcstr path)
	{
        ID3DBlob* pixelShader;
        ID3DBlob* errorBuff;

        wchar_t wPath[256];
        size_t result;
        mbstowcs_s(&result, wPath, path, 256);

        auto hResult = D3DCompileFromFile(wPath, nullptr, nullptr, "main", "ps_5_0",
#ifdef RAY_DEBUG
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else 
            0,
#endif
            0, & pixelShader, &errorBuff);

        if (FAILED(hResult))
        {
            OutputDebugStringA(static_cast<char*>(errorBuff->GetBufferPointer()));
            return false;
        }

        if (GetBytecode())
            delete static_cast<D3D12_SHADER_BYTECODE*>(GetBytecode());

        D3D12_SHADER_BYTECODE* bytecode = new D3D12_SHADER_BYTECODE;
        bytecode->BytecodeLength = pixelShader->GetBufferSize();
        bytecode->pShaderBytecode = pixelShader->GetBufferPointer();

        SetBytecode(bytecode);
        SetBuffer(pixelShader);

        errorBuff->Release();

        return true;
	}

	D3D12PixelShader::~D3D12PixelShader()
	{
		if (GetBytecode())
			delete GetBytecode();

		if (GetBuffer())
			static_cast<ID3DBlob*>(GetBuffer())->Release();
	}
}
