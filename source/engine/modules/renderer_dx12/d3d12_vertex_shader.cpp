#include "pch.hpp"
#include <cstdlib>
#include "d3d12_vertex_shader.hpp"

namespace ray::renderer::d3d12
{ 
	bool D3D12VertexShader::Compile(pcstr path)
	{
        ID3DBlob* vertexShader;
        ID3DBlob* errorBuff; 

        wchar_t wPath[256];
        size_t result;
        mbstowcs_s(&result, wPath, path, 256);

        auto hResult = D3DCompileFromFile(wPath, nullptr, nullptr, "main", "vs_5_0",
#ifdef RAY_DEBUG
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else 
            0,
#endif
            0, &vertexShader, &errorBuff);

        if (FAILED(hResult))
        {
            OutputDebugStringA(static_cast<char*>(errorBuff->GetBufferPointer()));
            return false;
        }

        if (GetBytecode())
            delete static_cast<D3D12_SHADER_BYTECODE*>(GetBytecode());

        D3D12_SHADER_BYTECODE* bytecode = new D3D12_SHADER_BYTECODE;
        bytecode->BytecodeLength = vertexShader->GetBufferSize();
        bytecode->pShaderBytecode = vertexShader->GetBufferPointer();
        
        SetBytecode(bytecode);
        SetBuffer(vertexShader);

        errorBuff->Release();

		return true;
	}

	D3D12VertexShader::~D3D12VertexShader()
	{
        if (GetBytecode())
            delete static_cast<D3D12_SHADER_BYTECODE*>(GetBytecode());

        if (GetBuffer())
            static_cast<ID3DBlob*>(GetBuffer())->Release();
	}
}