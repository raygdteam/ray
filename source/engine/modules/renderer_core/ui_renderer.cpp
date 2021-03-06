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
	LocalUiVertex() = default;

	LocalUiVertex(const ImDrawVert& vertex, u32 textureIndex)
		: Vertex(vertex)
		, TextureIndex(textureIndex)
	{}

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

	LocalUiVertex SceneRenderTargetVertices[6];
	FVector2 SceneRenderTargetTexCoords[4];
	u32 SceneRenderTargetColor = 0xffffffff;

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
	textureAtlasDesc.UploadBufferData = gUploadBuffer->SetTextureData(data, w, h);
	sUiData.TextureAtlas.Create(textureAtlasDesc, "UiRendererData::TextureAtlas");

	sUiData.TextureAtlasView.Create(sUiData.TextureAtlas, &_descriptorHeap);

	sUiData.SceneRenderTargetTexCoords[0] = { 0.f, 1.f }; // bottom left
	sUiData.SceneRenderTargetTexCoords[1] = { 0.f, 0.f }; // top left
	sUiData.SceneRenderTargetTexCoords[2] = { 1.f, 0.f }; // top right
	sUiData.SceneRenderTargetTexCoords[3] = { 1.f, 1.f }; // bottom right

	sUiData.SceneRenderTargetVertices[0].Vertex = 
	{ 
		{ -100.5f, -100.5f }, 
		{ 
			sUiData.SceneRenderTargetTexCoords[0].x,
			sUiData.SceneRenderTargetTexCoords[0].y
		},
		  sUiData.SceneRenderTargetColor 
	}; // bottom left
	sUiData.SceneRenderTargetVertices[0].TextureIndex = 1;

	sUiData.SceneRenderTargetVertices[1].Vertex =
	{
		{ -100.5f,  100.5f },
		{
			sUiData.SceneRenderTargetTexCoords[1].x,
			sUiData.SceneRenderTargetTexCoords[1].y
		},
		  sUiData.SceneRenderTargetColor
	}; // top left
	sUiData.SceneRenderTargetVertices[1].TextureIndex = 1;

	sUiData.SceneRenderTargetVertices[2].Vertex =
	{
		{ 100.5f,  100.5f },
		{
			sUiData.SceneRenderTargetTexCoords[2].x,
			sUiData.SceneRenderTargetTexCoords[2].y
		},
		  sUiData.SceneRenderTargetColor
	}; // top right
	sUiData.SceneRenderTargetVertices[2].TextureIndex = 1;

	sUiData.SceneRenderTargetVertices[3].Vertex =
	{
		{ -100.5f, -100.5f },
		{
			sUiData.SceneRenderTargetTexCoords[0].x,
			sUiData.SceneRenderTargetTexCoords[0].y
		},
		  sUiData.SceneRenderTargetColor
	}; // bottom left
	sUiData.SceneRenderTargetVertices[3].TextureIndex = 1;

	sUiData.SceneRenderTargetVertices[4].Vertex =
	{
		{ 100.5f, -100.5f },
		{
			sUiData.SceneRenderTargetTexCoords[3].x,
			sUiData.SceneRenderTargetTexCoords[3].y
		},
		  sUiData.SceneRenderTargetColor
	}; // bottom right
	sUiData.SceneRenderTargetVertices[4].TextureIndex = 1;

	sUiData.SceneRenderTargetVertices[5].Vertex =
	{
		{ 100.5f,  100.5f },
		{
			sUiData.SceneRenderTargetTexCoords[2].x,
			sUiData.SceneRenderTargetTexCoords[2].y
		},
		  sUiData.SceneRenderTargetColor
	}; // top right
	sUiData.SceneRenderTargetVertices[5].TextureIndex = 1;

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

void UiRenderer::Begin(const FMatrix4x4& vp, GraphicsContext& gfxContext) noexcept
{
	sUiData.ViewProjection = vp;
	
	sUiData.SceneRenderTarget = &gSceneColorBuffer;

	u32 range[] = { 1 };
	auto destHandle = _descriptorHeap.GetDescriptorAtOffset(1).GetCpuHandle();
	auto srcHandle = gSceneColorBuffer.GetTextureView().GetSRV();
	gDevice->CopyDescriptors(1, &destHandle, range, 1, &srcHandle, range, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	gfxContext.TransitionResource(*sUiData.SceneRenderTarget, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, true);

	for (size_t i = 0; i < 6; ++i)
	{
		sUiData.VertexBufferBase[sUiData.VertexCount + i] = sUiData.SceneRenderTargetVertices[i];
	}

	gfxContext.SetRootSignature(_uiRootSignature);
	gfxContext.SetScissor(0, 0, gDisplayPlane->GetDesc().Width, gDisplayPlane->GetDesc().Height);
	gfxContext.SetViewport(0.f, 0.f, gDisplayPlane->GetDesc().Width, gDisplayPlane->GetDesc().Height);

	gfxContext.SetPipelineState(_uiPipelineState);
	gfxContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gfxContext.SetRenderTarget(gEditorColorBuffer.GetRTV(), gDepthBuffer.GetDSV());

	gfxContext.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, _descriptorHeap.GetHeapPointer());
	gfxContext.SetDescriptorTable(0, _descriptorHeap.GetDescriptorAtOffset(0).GetGpuHandle());

	UiConstantBuffer cb = { sUiData.ViewProjection.Transpose() };

	gfxContext.SetDynamicVB(gRingBuffer, 0, sUiData.VertexCount + 6, sizeof(LocalUiVertex), sUiData.VertexBufferBase);
	gfxContext.SetDynamicIB(gRingBuffer, sUiData.IndexCount, sUiData.IndexBufferBase, true);
	gfxContext.SetDynamicCBV(gRingBuffer, 1, sizeof(cb), &cb);
}

void UiRenderer::Draw(size_t indexCount, size_t vertexOffset, size_t indexOffset, GraphicsContext& gfxContext) noexcept
{	
	gfxContext.DrawIndexedInstanced(indexCount, 1, indexOffset, vertexOffset, 0);
}

void UiRenderer::DrawSceneRenderTarget(const FVector3& pos, const FVector2& size, GraphicsContext& gfxContext) noexcept
{
	// TODO: calculates wrong position

	for (size_t i = 0; i < 6; i++)
	{
		FVector3 p =
		{
			sUiData.SceneRenderTargetVertices[i].Vertex.pos.x,
			sUiData.SceneRenderTargetVertices[i].Vertex.pos.y,
			1.f
		};
		FVector3 vertexPos = p * FVector3{ size.x, size.y, 1.f };
		FVector3 newPosition = pos + vertexPos;

		sUiData.VertexBufferBase[sUiData.VertexCount + i] = sUiData.SceneRenderTargetVertices[i];
		sUiData.VertexBufferBase[sUiData.VertexCount + i].Vertex.pos = { newPosition.x, newPosition.y };
	}

	gfxContext.SetDynamicVB(gRingBuffer, 0, 6, sizeof(LocalUiVertex), &sUiData.VertexBufferBase[sUiData.VertexCount]);
	gfxContext.Draw(6, 0);
}

void UiRenderer::End(GraphicsContext& gfxContext) noexcept
{
	gfxContext.Flush();

	sUiData.IndexCount = 0;
	sUiData.VertexCount = 0;
}

void UiRenderer::SetVertices(ImDrawVert* vertices, size_t count) noexcept
{
	for (size_t i = 0; i < count; ++i)
	{
		sUiData.VertexBufferBase[sUiData.VertexCount + i].Vertex = vertices[i];
		sUiData.VertexBufferBase[sUiData.VertexCount + i].TextureIndex = 0;
	}

	sUiData.VertexCount += count;
}

void UiRenderer::SetIndices(u32* indices, size_t count) noexcept
{
	memcpy(&sUiData.IndexBufferBase[sUiData.IndexCount], indices, count * sizeof(u32));
	sUiData.IndexCount += count;
}
