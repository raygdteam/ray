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
	ImDrawVert Vertex;
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
	
	ColorBuffer* SceneRenderTarget = nullptr;

	u32* IndexBufferBase = nullptr;
	size_t IndexCount = 0;

	LocalUiVertex* VertexBufferBase = nullptr;
	size_t VertexCount = 0;

	FVector3 VertexPositions[4];

	FMatrix4x4 ViewProjection;
};

struct UiConstantBuffer
{
	FMatrix4x4 ViewProjMatrix;
};

static UiRendererData sUiData;

GraphicsPipeline UiRenderer::_uiPipelineState;
RootSignature UiRenderer::_uiRootSignature;
DescriptorHeap UiRenderer::_descriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 64);

void UiRenderer::Initialize(u32 w, u32 h, void* data) noexcept
{
	sUiData.IndexBufferBase = new uint32_t[sUiData.MAX_INDICES];
	sUiData.VertexBufferBase = new LocalUiVertex[sUiData.MAX_VERTICES];

	_descriptorHeap.Create();

	//auto textureResolution = FVector2 { w, h };
	auto textureAtlasDesc = GpuTextureDescription::Texture2D(w, h, DXGI_FORMAT_R8G8B8A8_UNORM, 1, D3D12_RESOURCE_FLAG_NONE);
	textureAtlasDesc.UploadBufferData = gUploadBuffer->SetBufferData(data, w * h, sizeof(u32));
	sUiData.TextureAtlas.Create(textureAtlasDesc, "UiRendererData::TextureAtlas");

	sUiData.TextureAtlasView.Create(sUiData.TextureAtlas, &_descriptorHeap);

	sUiData.VertexPositions[0] = { -100.5f, -100.5f, 0.5f }; // bottom left
	sUiData.VertexPositions[1] = { -100.5f,  100.5f, 0.5f }; // top left
	sUiData.VertexPositions[2] = { 100.5f,  100.5f, 0.5f }; // top right
	sUiData.VertexPositions[3] = { 100.5f, -100.5f, 0.5f }; // bottom right

	SamplerDesc defaultSampler;
	defaultSampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	defaultSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	defaultSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	defaultSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	defaultSampler.MipLODBias = 0.f;
	defaultSampler.MaxAnisotropy = 0;
	defaultSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	defaultSampler.MinLOD = 0.f;
	defaultSampler.MaxLOD = 0.f;

	_uiRootSignature.Begin(2, 1);
	_uiRootSignature.InitStaticSampler(0, defaultSampler, D3D12_SHADER_VISIBILITY_PIXEL);
	_uiRootSignature.Slot(0).InitAsDescriptorRange(0, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, D3D12_SHADER_VISIBILITY_PIXEL);
	_uiRootSignature.Slot(1).InitAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_VERTEX);

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

	// Create the blending setup
	{
		D3D12_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		_uiPipelineState.SetBlendState(desc);

	}

	// Create the rasterizer state
	{
		D3D12_RASTERIZER_DESC desc = {};
		desc.FillMode = D3D12_FILL_MODE_SOLID;
		desc.CullMode = D3D12_CULL_MODE_NONE;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = FALSE;
		desc.AntialiasedLineEnable = FALSE;
		desc.ForcedSampleCount = 0;
		desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		_uiPipelineState.SetRasterizerState(desc);
	}

	// Create depth-stencil State
	{
		D3D12_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		desc.BackFace = desc.FrontFace;
		_uiPipelineState.SetDepthStencilState(desc);
	}

	_uiPipelineState.SetDSVFormat(gDepthBuffer.GetDesc().Format);

	_uiPipelineState.Finalize();
}

void UiRenderer::Begin(const FMatrix4x4& vp) noexcept
{
	sUiData.ViewProjection = vp;
	
	sUiData.SceneRenderTarget = &gSceneColorBuffer;

	sUiData.IndexCount = 0;
	sUiData.VertexCount = 0;

	u32 range[] = { 1 };
	auto destHandle = _descriptorHeap.GetDescriptorAtOffset(1).GetCpuHandle();
	auto srcHandle = gSceneColorBuffer.GetTextureView().GetSRV();
	gDevice->CopyDescriptors(1, &destHandle, range, 1, &srcHandle, range, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void UiRenderer::Draw(ImDrawVert* vertices, size_t verticesCount, u32* indices, size_t indicesCount, GraphicsContext& gfxContext) noexcept
{	
	if (sUiData.IndexCount + indicesCount > sUiData.MAX_INDICES)
		FlushAndReset(gfxContext);

	for (size_t i = 0; i < verticesCount; ++i)
	{
		/*sUiData.VertexBufferPtr->Vertex.Position = vertices[i].Position;
		sUiData.VertexBufferPtr->Vertex.Color = vertices[i].Color;
		sUiData.VertexBufferPtr->Vertex.TexCoords = vertices[i].TexCoords;*/
		sUiData.VertexBufferBase[sUiData.VertexCount + i].Vertex = vertices[i];
		sUiData.VertexBufferBase[sUiData.VertexCount + i].TextureIndex = 0;
	}

	sUiData.VertexCount += verticesCount;

	memcpy(&sUiData.IndexBufferBase[sUiData.IndexCount], indices, indicesCount * sizeof(u32));
	sUiData.IndexCount += indicesCount;
}

void UiRenderer::DrawSceneRenderTarget(const FVector3& pos, const FVector2& size, GraphicsContext& gfxContext) noexcept
{
	if (sUiData.IndexCount + 6 > sUiData.MAX_INDICES)
		FlushAndReset(gfxContext);

	constexpr size_t quadVertexCount = 4;

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		FVector3 vertexPos = sUiData.VertexPositions[i] * FVector3{ size.x, size.y, 1.f };
		FVector3 newPosition = pos + vertexPos;

		sUiData.VertexBufferBase[sUiData.VertexCount + i].Vertex.pos.x = newPosition.x;
		sUiData.VertexBufferBase[sUiData.VertexCount + i].Vertex.pos.y = newPosition.y;
		sUiData.VertexBufferBase[sUiData.VertexCount + i].TextureIndex = 1;
	}

	sUiData.IndexBufferBase[sUiData.IndexCount] = sUiData.IndexCount + 0;
	sUiData.IndexBufferBase[sUiData.IndexCount + 1] = sUiData.IndexCount + 1;
	sUiData.IndexBufferBase[sUiData.IndexCount + 2] = sUiData.IndexCount + 2;

	sUiData.IndexBufferBase[sUiData.IndexCount + 3] = sUiData.IndexCount + 2;
	sUiData.IndexBufferBase[sUiData.IndexCount + 4] = sUiData.IndexCount + 3;
	sUiData.IndexBufferBase[sUiData.IndexCount + 5] = sUiData.IndexCount + 0;
}

void UiRenderer::End(GraphicsContext& gfxContext) noexcept
{
	if (sUiData.VertexCount != 0)
	{
		Flush(gfxContext);
	}
}

void UiRenderer::Flush(GraphicsContext& gfxContext) noexcept
{
	if (sUiData.SceneRenderTarget)
	{
		gfxContext.TransitionResource(*sUiData.SceneRenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, true);
	}

	gfxContext.SetRootSignature(_uiRootSignature);
	gfxContext.SetScissor(0, 0, 1280, 720);
	gfxContext.SetViewport(0.f, 0.f, 1280.f, 720.f);

	gfxContext.SetPipelineState(_uiPipelineState);
	gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gfxContext.SetRenderTarget(gEditorColorBuffer.GetRTV(), gDepthBuffer.GetDSV());

	gfxContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, _descriptorHeap.GetHeapPointer());
	gfxContext.SetDescriptorTable(0, _descriptorHeap.GetDescriptorAtOffset(0).GetGpuHandle());

	UiConstantBuffer cb = { sUiData.ViewProjection.Transpose() };
	
	gfxContext.SetDynamicVB(gRingBuffer, 0, sUiData.VertexCount, sizeof(LocalUiVertex), sUiData.VertexBufferBase);
	gfxContext.SetDynamicIB(gRingBuffer, sUiData.IndexCount, sUiData.IndexBufferBase, true);
	gfxContext.SetDynamicCBV(gRingBuffer, 1, sizeof(cb), &cb);
	gfxContext.DrawIndexedInstanced(sUiData.IndexCount, 1, 0, 0, 0);

	gfxContext.Flush();
}

void UiRenderer::FlushAndReset(GraphicsContext& gfxContext) noexcept
{
	Flush(gfxContext);
	Begin(sUiData.ViewProjection);
}
