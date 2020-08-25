#include "renderer_2d.hpp"
#include "ray_renderer_core_class_helper.hpp"
#include <engine/state/state.hpp>
#include <core/lib/array.hpp>
#include <core/math/matrix.hpp>
#include "resources/gpu_buffer.hpp"
#include "pipeline_state.hpp"
#include "root_signature.hpp"
#include <d3dcompiler.h>
#include "d3dx12.h"
#include "command_context.hpp"
#include "resources/buffer_manager.hpp"

namespace ray::renderer_core_api
{
	RootSignature Renderer2D::_2DSignature;
	GraphicsPipeline Renderer2D::_2DPipeline;

	struct QuadVertex
	{
		FVector<3> Position;
		FVector<4> Color;
	};

	struct Renderer2DData
	{
		Renderer2DData() {}

		static const u32 MAX_QUADS = 20000;
		static const u32 MAX_VERTICES = MAX_QUADS * 4;
		static const u32 MAX_INDICES = MAX_QUADS * 6;

		resources::GpuBuffer IndexBuffer;
		D3D12_INDEX_BUFFER_VIEW IndexBufferView;

		u32 QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		FVector<3> QuadVertexPositions[4];
	};

	static Renderer2DData sData;

	void Renderer2D::Initialize()
	{
		u32* quadIndices = new uint32_t[sData.MAX_INDICES];
		u32 offset = 0;
		for (uint32_t i = 0; i < sData.MAX_INDICES; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 0;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 1;

			offset += 4;
		}

		sData.IndexBuffer.Create(sData.MAX_INDICES, sizeof(u32), quadIndices);
		sData.IndexBufferView = sData.IndexBuffer.IndexBufferView(0);
		delete[] quadIndices;

		sData.QuadVertexBufferBase = new QuadVertex[sData.MAX_QUADS];

		sData.QuadVertexPositions[0] = { -0.5f,  0.5f, 0.5f };
		sData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.5f };
		sData.QuadVertexPositions[2] = { -0.5f, -0.5f, 0.5f };
		sData.QuadVertexPositions[3] = { 0.5f,  0.5f, 0.5f };

		_2DSignature.Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		_2DPipeline.SetRootSignature(_2DSignature);

		ID3DBlob* vertexShader;
		ID3DBlob* errorBuff;
		auto hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuff);
		check(hr == S_OK)

			D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
		vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
		vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

		ID3DBlob* pixelShader;
		hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuff);
		check(hr == S_OK)

		D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
		pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
		pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

		_2DPipeline.SetVertexShader(vertexShaderBytecode);
		_2DPipeline.SetPixelShader(pixelShaderBytecode);

		D3D12_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};
		_2DPipeline.SetInputLayout(2, inputLayout);
		_2DPipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		_2DPipeline.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
		_2DPipeline.SetSampleMask(0xffffffff);
		_2DPipeline.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
		_2DPipeline.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
		_2DPipeline.Finalize();
	}

	void Renderer2D::Begin()
	{
		sData.QuadIndexCount = 0;
		sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
	}

	void Renderer2D::End(GraphicsContext& gfxContext)
	{
		Flush(gfxContext);
	}

	void Renderer2D::DrawQuad(const FVector<3>& pos, const FVector<2>& size, const FVector<4>& color, GraphicsContext& gfxContext)
	{
		auto mat = FMatrix4x4::Scale(FVector<3>{ size.x, size.y, 0.f });
		auto position = mat.Transform(FVector<4>{ pos.x, pos.y, pos.z, 1.f });

		DrawQuad(FVector<3>{ position.x, position.y, position.z }, color, gfxContext);
	}

	void Renderer2D::DrawQuad(const FVector<3>& pos, const FVector<4>& color, GraphicsContext& gfxContext)
	{
		constexpr size_t quadVertexCount = 4;

		if (sData.QuadIndexCount >= sData.MAX_INDICES)
			FlushAndReset(gfxContext);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			auto vertexPos = sData.QuadVertexPositions[i];
			FVector<3> newPosition = pos + vertexPos;
			FVector<3> newPosition1 = { pos.x + vertexPos.x, pos.y + vertexPos.y, pos.z + vertexPos.z };
			ray_assert(newPosition.x == newPosition1.x && newPosition.y == newPosition1.y && newPosition.z == newPosition1.z, "catastropic failure: instruction unclear: planet annihilated");
			
			sData.QuadVertexBufferPtr->Position = newPosition;
			sData.QuadVertexBufferPtr->Color = color;
			sData.QuadVertexBufferPtr++;
		}

		sData.QuadIndexCount += 6;
	}

	void Renderer2D::Flush(GraphicsContext& gfxContext)
	{
		// assuming that RTV is already transferred to D3D12_RESOURCE_STATE_RENDER_TARGET

		gfxContext.SetRootSignature(_2DSignature);
		gfxContext.SetScissor(0, 0, 1280, 720);
		gfxContext.SetViewport(0.f, 0.f, 1280.f, 720.f);

		gfxContext.SetPipelineState(_2DPipeline);
		gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		size_t bufferSize = sData.QuadVertexBufferPtr - sData.QuadVertexBufferBase;
		gfxContext.SetDynamicVB(0, bufferSize, sizeof(QuadVertex), sData.QuadVertexBufferBase);
		gfxContext.SetIndexBuffer(sData.IndexBufferView);
		gfxContext.DrawIndexedInstanced(sData.QuadIndexCount, 1, 0, 0, 0);

		gfxContext.Flush();
	}

	void Renderer2D::FlushAndReset(GraphicsContext& gfxContext)
	{
		Flush(gfxContext);
		Begin();
	}

	void Renderer2D::Shutdown()
	{
		sData.IndexBuffer.Destroy();
		delete[] sData.QuadVertexBufferBase;
	}

	Renderer2D::~Renderer2D()
	{

	}
}