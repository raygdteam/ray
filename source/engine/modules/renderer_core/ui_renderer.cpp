#include "ui_renderer.hpp"
#include "resources/color_buffer.hpp"
#include "resources/gpu_buffer.hpp"
#include "resources/gpu_texture.hpp"
#include "pipeline_state.hpp"
#include "root_signature.hpp"
#include "renderer.hpp"
#include "resources/upload_buffer.hpp"
#include "descriptor_heap.hpp"
#include "sampler_manager.hpp"
#include <d3d12.h>
#include <d3dcompiler.h>
#include "resources/buffer_manager.hpp"
#include "resources/depth_buffer.hpp"
#include "command_context.hpp"

struct LocalUiVertex
{
	UiVertex Vertex;
	u32 TextureIndex;
};

struct UiRendererData
{
	UiRendererData() {}

	static const u32 MAX_QUADS = 20000;
	static const u32 MAX_VERTICES = MAX_QUADS * 4;
	static const u32 MAX_INDICES = MAX_QUADS * 6;

	GpuTexture TextureAtlas;
	TextureView TextureAtlasView;
	
	ColorBuffer* EngineRenderTarget;

	u32* IndexBufferBase = nullptr;
	u32* IndexBufferPtr = nullptr;

	LocalUiVertex* VertexBufferBase = nullptr;
	LocalUiVertex* VertexBufferPtr = nullptr;

	FVector3 VertexPositions[4];
};

static UiRendererData sUiData;

GraphicsPipeline UiRenderer::_uiPipelineState;
RootSignature UiRenderer::_uiRootSignature;
DescriptorHeap UiRenderer::_descriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 64);

void UiRenderer::Initialize(RTexture& textureAtlas) noexcept
{
	sUiData.IndexBufferBase = new uint32_t[sUiData.MAX_INDICES];

	_descriptorHeap.Create();

	auto textureResolution = textureAtlas.GetDimensions();
	auto textureAtlasDesc = GpuTextureDescription::Texture2D(textureResolution.x, textureResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, 1, D3D12_RESOURCE_FLAG_NONE);
	textureAtlasDesc.UploadBufferData = gUploadBuffer->SetTextureData(textureAtlas);
	sUiData.TextureAtlas.Create(textureAtlasDesc);

	sUiData.TextureAtlasView.Create(sUiData.TextureAtlas, &_descriptorHeap);

	sUiData.VertexBufferBase = new LocalUiVertex[sUiData.MAX_QUADS];

	sUiData.VertexPositions[0] = { -100.5f, -100.5f, 0.5f }; // bottom left
	sUiData.VertexPositions[1] = { -100.5f,  100.5f, 0.5f }; // top left
	sUiData.VertexPositions[2] = { 100.5f,  100.5f, 0.5f }; // top right
	sUiData.VertexPositions[3] = { 100.5f, -100.5f, 0.5f }; // bottom right

	SamplerDesc defaultSampler;
	_uiRootSignature.Begin(1, 1);
	_uiRootSignature.InitStaticSampler(0, defaultSampler, D3D12_SHADER_VISIBILITY_PIXEL);
	_uiRootSignature.Slot(0).InitAsDescriptorRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, D3D12_SHADER_VISIBILITY_PIXEL);

	_uiRootSignature.Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	_uiPipelineState.SetRootSignature(_uiRootSignature);

	ID3DBlob* vertexShader;
	ID3DBlob* errorBuff;
	auto hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\UiRenderer_VS.hlsl", nullptr, nullptr, "main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuff);
	check(hr == S_OK)

		D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

	ID3DBlob* pixelShader;
	hr = D3DCompileFromFile(L"..\\..\\source\\engine\\modules\\renderer_core\\UiRenderer_PS.hlsl", nullptr, nullptr, "main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuff);
	check(hr == S_OK)

		D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	_uiPipelineState.SetVertexShader(vertexShaderBytecode);
	_uiPipelineState.SetPixelShader(pixelShaderBytecode);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	_uiPipelineState.SetInputLayout(sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC), inputLayout);
	_uiPipelineState.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	_uiPipelineState.SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	_uiPipelineState.SetSampleMask(0xffffffff);
	_uiPipelineState.SetRasterizerState(CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT));
	_uiPipelineState.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));

	_uiPipelineState.SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
	_uiPipelineState.SetDSVFormat(gDepthBuffer.GetDesc().Format);

	_uiPipelineState.Finalize();
}

void UiRenderer::Begin(ColorBuffer& engineRenderTarget) noexcept
{
	sUiData.EngineRenderTarget = &engineRenderTarget;

	sUiData.IndexBufferPtr = sUiData.IndexBufferBase;
	sUiData.VertexBufferPtr = sUiData.VertexBufferBase;

	u32 range[] = { 1 };
	auto destHandle = _descriptorHeap.GetDescriptorAtOffset(1).GetCpuHandle();
	auto srcHandle = engineRenderTarget.GetTextureView().GetSRV();
	gDevice->CopyDescriptors(1, &destHandle, range, 1, &srcHandle, range, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void UiRenderer::Draw(UiVertex* vertices, size_t verticesCount, u32* indices, size_t indicesCount, GraphicsContext& gfxContext) noexcept
{

	if (reinterpret_cast<size_t>(reinterpret_cast<size_t*>(sUiData.IndexBufferPtr - sUiData.IndexBufferBase + indicesCount)) > sUiData.MAX_INDICES)
		FlushAndReset(gfxContext);

	for (size_t i = 0; i < verticesCount; ++i)
	{
		sUiData.VertexBufferPtr->Vertex = vertices[i];
		sUiData.VertexBufferPtr->TextureIndex = 0;
		sUiData.VertexBufferPtr++;
	}

	memcpy(sUiData.IndexBufferPtr, indices, indicesCount);
	sUiData.IndexBufferPtr += indicesCount;
}



void UiRenderer::DrawEngineRenderTarget(const FVector3& pos, const FVector2& size, GraphicsContext& gfxContext) noexcept
{
	size_t indexCount = sUiData.IndexBufferPtr - sUiData.IndexBufferBase;

	if (reinterpret_cast<size_t>(reinterpret_cast<size_t*>(sUiData.IndexBufferPtr - sUiData.IndexBufferBase + indexCount)) > sUiData.MAX_INDICES)
		FlushAndReset(gfxContext);

	constexpr size_t quadVertexCount = 4;

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		FVector3 vertexPos = sUiData.VertexPositions[i] * FVector3{ size.x, size.y, 1.f };
		FVector3 newPosition = pos + vertexPos;

		sUiData.VertexBufferPtr->Vertex.Position = FVector2{ newPosition.x, newPosition.y };
		sUiData.VertexBufferPtr->TextureIndex = 1;
		sUiData.VertexBufferPtr++;
	}

	*  sUiData.IndexBufferPtr = indexCount + 0;
	*++sUiData.IndexBufferPtr = indexCount + 1;
	*++sUiData.IndexBufferPtr = indexCount + 2;

	*++sUiData.IndexBufferPtr = indexCount + 2;
	*++sUiData.IndexBufferPtr = indexCount + 3;
	*++sUiData.IndexBufferPtr = indexCount + 0;
}

void UiRenderer::End(GraphicsContext& gfxContext) noexcept
{
	Flush(gfxContext);
}

void UiRenderer::Flush(GraphicsContext& gfxContext) noexcept
{
	gfxContext.TransitionResource(*sUiData.EngineRenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, true);

	gfxContext.SetRootSignature(_uiRootSignature);
	gfxContext.SetScissor(0, 0, 1280, 720);
	gfxContext.SetViewport(0.f, 0.f, 1280.f, 720.f);

	gfxContext.SetPipelineState(_uiPipelineState);
	gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gfxContext.SetRenderTarget(gDisplayPlane[gCurrentBuffer].GetRTV(), gDepthBuffer.GetDSV());

	gfxContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, _descriptorHeap.GetHeapPointer());
	gfxContext.SetDescriptorTable(0, _descriptorHeap.GetDescriptorAtOffset(0).GetGpuHandle());


	size_t bufferSize = sUiData.VertexBufferPtr - sUiData.VertexBufferBase;
	size_t indexCount = sUiData.IndexBufferPtr - sUiData.IndexBufferBase;
	gfxContext.SetDynamicVB(gRingBuffer, 0, bufferSize, sizeof(UiVertex), sUiData.VertexBufferBase);
	gfxContext.SetDynamicIB(gRingBuffer, indexCount, sUiData.IndexBufferBase, true);
	gfxContext.DrawIndexedInstanced(indexCount, 1, 0, 0, 0);

	gfxContext.Flush();
}

void UiRenderer::FlushAndReset(GraphicsContext& gfxContext) noexcept
{
	Flush(gfxContext);
	Begin(*sUiData.EngineRenderTarget);
}
