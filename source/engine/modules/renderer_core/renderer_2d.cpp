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
#include "sampler_manager.hpp"
#include "resources/gpu_texture.hpp"
#include <resources/resource.hpp>
#include "descriptor_heap.hpp"
#include "renderer.hpp"
#include "resources/upload_buffer.hpp"
#include "resources/ring_buffer.hpp"
#include <engine/world/world.hpp>

RootSignature Renderer2D::_2DSignature;
GraphicsPipeline Renderer2D::_2DPipeline;
DescriptorHeap Renderer2D::_descriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1);

struct ConstantBuffer
{
	FMatrix4x4 ViewProjMatrix;
};

struct QuadVertex
{
	FVector4 Position;
	FVector4 Color;
	FVector2 TexCoord;
	u32 TextureIndex;
};

struct Renderer2DData
{
	Renderer2DData() {}

	static const u32 MAX_QUADS = 20000;
	static const u32 MAX_VERTICES = MAX_QUADS * 4;
	static const u32 MAX_INDICES = MAX_QUADS * 6;
	static const u32 MAX_TEXTURES_COUNT = 16;

	GpuBuffer IndexBuffer;
	GpuTexture WhiteTexture;
	TextureView WhiteTextureView;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;

	u32 QuadIndexCount = 0;
	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE AttachedTextures[MAX_TEXTURES_COUNT];
	u32 TextureCount = 0;

	FVector3 QuadVertexPositions[4];

	FMatrix4x4 ViewProjectionMatrix;
};

static Renderer2DData sData;

void Renderer2D::Initialize(RTexture& whiteTexture)
{
	u32* quadIndices = new uint32_t[sData.MAX_INDICES];
	u32 offset = 0;
	for (uint32_t i = 0; i < sData.MAX_INDICES; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	auto ibDesc = GpuBufferDescription::Index(sData.MAX_INDICES, sizeof(u32), quadIndices);
	ibDesc.UploadBufferData = gUploadBuffer->SetBufferData(quadIndices, sData.MAX_INDICES, sizeof(u32));
	ibDesc.Flags = D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
	sData.IndexBuffer.Create(ibDesc, "Renderer2DData::IndexBuffer");

	BufferView ibView;
	ibView.Create(sData.IndexBuffer);
	sData.IndexBufferView = ibView.GetIndexBufferView();
	delete[] quadIndices;

	auto textureResolution = whiteTexture.GetDimensions();
	auto whiteTextureDesc = GpuTextureDescription::Texture2D(textureResolution.x, textureResolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_RESOURCE_FLAG_NONE);
	whiteTextureDesc.UploadBufferData = gUploadBuffer->SetTextureData(whiteTexture);
	sData.WhiteTexture.Create(whiteTextureDesc, "Renderer2DData::WhiteTexture");

	sData.WhiteTextureView.Create(sData.WhiteTexture);
	sData.TextureCount = 1;
	sData.AttachedTextures[0] = sData.WhiteTextureView.GetSRV();

	_descriptorHeap.Create();

	sData.QuadVertexBufferBase = new QuadVertex[sData.MAX_QUADS];

	sData.QuadVertexPositions[0] = { -100.5f, -100.5f, 0.5f }; // bottom left
	sData.QuadVertexPositions[1] = { -100.5f,  100.5f, 0.5f }; // top left
	sData.QuadVertexPositions[2] = { 100.5f,  100.5f, 0.5f }; // top right
	sData.QuadVertexPositions[3] = { 100.5f, -100.5f, 0.5f }; // bottom right

	SamplerDesc defaultSampler;
	_2DSignature.Begin(2, 1);
	_2DSignature.InitStaticSampler(0, defaultSampler, D3D12_SHADER_VISIBILITY_PIXEL);
	_2DSignature.Slot(0).InitAsDescriptorRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 64, D3D12_SHADER_VISIBILITY_PIXEL);
	_2DSignature.Slot(1).InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);

	_2DSignature.Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	_2DPipeline.SetRootSignature(_2DSignature);

	ID3DBlob* vertexShader;
	ID3DBlob* errorBuff;
	auto hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\Renderer2D_VS.hlsl", nullptr, nullptr, "main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuff);
	check(hr == S_OK)

		D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

	ID3DBlob* pixelShader;
	hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\Renderer2D_PS.hlsl", nullptr, nullptr, "main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuff);
	check(hr == S_OK)

		D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	_2DPipeline.SetVertexShader(vertexShaderBytecode);
	_2DPipeline.SetPixelShader(pixelShaderBytecode);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	_2DPipeline.SetInputLayout(sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC), inputLayout);
	_2DPipeline.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	_2DPipeline.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	_2DPipeline.SetSampleMask(0xffffffff);
	_2DPipeline.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
	_2DPipeline.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));

	_2DPipeline.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	_2DPipeline.SetDSVFormat(gDepthBuffer.GetDesc().Format);

	_2DPipeline.Finalize();
}

void Renderer2D::Begin(const FMatrix4x4& viewProjection)
{
	sData.ViewProjectionMatrix = viewProjection;

	Begin();
}

void Renderer2D::Begin()
{
	sData.TextureCount = 1;
	sData.QuadIndexCount = 0;
	sData.QuadVertexBufferPtr = sData.QuadVertexBufferBase;
}

void Renderer2D::End(GraphicsContext& gfxContext)
{
	Flush(gfxContext);
}

/*void Renderer2D::DrawQuad(const FVector3& pos, const FVector2& size, FVector2* textureCoords, GraphicsContext& gfxContext)
{
	auto mat = FMatrix4x4::Scale(FVector3{ size.x, size.y, 1.f });
	auto position = mat.Transform(FVector4{ pos.x, pos.y, pos.z, 1.f });

	DrawQuad(FVector3{ position.x, position.y, position.z }, textureCoords, gfxContext);
}*/

void Renderer2D::DrawQuad(const FVector3& pos, const FVector2& size, const TextureView& textureHandle, FVector2* textureCoords, GraphicsContext& gfxContext)
{
	constexpr size_t quadVertexCount = 4;

	// TODO: 
	if (sData.QuadIndexCount >= sData.MAX_INDICES || sData.TextureCount + 1 >= Renderer2DData::MAX_TEXTURES_COUNT)
		FlushAndReset(gfxContext);

	sData.AttachedTextures[sData.TextureCount] = textureHandle.GetSRV();

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		FVector3 vertexPos = sData.QuadVertexPositions[i] * FVector3{ size.x, size.y, 1.f };
		// vertexPos.z = .5f;

		/*FVector4 transformed = FMatrix4x4::Scale(1).Transform({ vertexPos.x, vertexPos.y, vertexPos.z, 1.f });
		vertexPos = { transformed.x, transformed.y, transformed.z };*/

		FVector3 newPosition = pos + vertexPos;

		sData.QuadVertexBufferPtr->Position = FVector4{ newPosition.x, newPosition.y, newPosition.z, 1.f };
		
		sData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		sData.QuadVertexBufferPtr->TextureIndex = sData.TextureCount++;

		sData.QuadVertexBufferPtr++;
	}

	sData.QuadIndexCount += 6;
}

void Renderer2D::DrawQuad(const FVector3& pos, const FVector2& size, const TextureView& textureHandle, GraphicsContext& gfxContext)
{
	static FVector2 textureCoords[4] =
	{
		{ 0.f, 1.f },
		{ 0.f, 0.f },
		{ 1.f, 0.f },
		{ 1.f, 1.f }
	};

	DrawQuad(pos, size, textureHandle, textureCoords, gfxContext);
}

void Renderer2D::DrawQuad(const FVector3& pos, const FVector2& size, const FVector4& color, GraphicsContext& gfxContext)
{
	constexpr size_t quadVertexCount = 4;

	static FVector2 textureCoords[4] =
	{
		{ 0.f, 1.f },
		{ 0.f, 0.f },
		{ 1.f, 0.f },
		{ 1.f, 1.f }
	};

	if (sData.QuadIndexCount + 6 > sData.MAX_INDICES)
		FlushAndReset(gfxContext);

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		FVector3 vertexPos = sData.QuadVertexPositions[i] * FVector3{ size.x, size.y, 1.f };
		FVector3 newPosition = pos + vertexPos;

		sData.QuadVertexBufferPtr->Position = FVector4{ newPosition.x, newPosition.y, newPosition.z, 1.f };
		sData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		sData.QuadVertexBufferPtr->Color = color;
		sData.QuadVertexBufferPtr->TextureIndex = 0;
		sData.QuadVertexBufferPtr++;
	}

	sData.QuadIndexCount += 6;
}

void Renderer2D::Flush(GraphicsContext& gfxContext)
{
	// assuming that RTV is already transferred to D3D12_RESOURCE_STATE_RENDER_TARGET

	u32 srcRange[Renderer2DData::MAX_TEXTURES_COUNT];
	u32 destRange = sData.TextureCount;
	for (size_t i = 0; i < Renderer2DData::MAX_TEXTURES_COUNT; ++i)
	{
		srcRange[i] = 1;
	}
	auto destHandle = _descriptorHeap.GetDescriptorAtOffset(0).GetCpuHandle();
	auto srcHandle = sData.AttachedTextures;
	gDevice->CopyDescriptors(1, &destHandle, &destRange, sData.TextureCount, srcHandle, srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	gfxContext.SetRootSignature(_2DSignature);
	gfxContext.SetScissor(0, 0, 1280, 720);
	gfxContext.SetViewport(0.f, 0.f, 1280.f, 720.f);

	gfxContext.SetPipelineState(_2DPipeline);
	gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gfxContext.SetRenderTarget(gSceneColorBuffer.GetRTV(), gDepthBuffer.GetDSV());

	ConstantBuffer cb;
	cb.ViewProjMatrix = sData.ViewProjectionMatrix.Transpose();

	gfxContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, gMainDescriptorHeap.GetHeapPointer());
	gfxContext.SetDescriptorTable(0, _descriptorHeap.GetDescriptorAtOffset(0).GetGpuHandle());

	size_t bufferSize = sData.QuadVertexBufferPtr - sData.QuadVertexBufferBase;
	gfxContext.SetDynamicVB(gRingBuffer, 0, bufferSize, sizeof(QuadVertex), sData.QuadVertexBufferBase);
	gfxContext.SetDynamicCBV(gRingBuffer, 1, sizeof(cb), &cb);
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
	sData.WhiteTexture.Destroy();
	gRingBuffer.Destroy();
	delete[] sData.QuadVertexBufferBase;
}

Renderer2D::~Renderer2D()
{

}
