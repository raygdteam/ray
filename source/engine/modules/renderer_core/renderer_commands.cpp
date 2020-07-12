#include "renderer_commands.hpp"
#include "vertex_shader.hpp"
#include "pixel_shader.hpp"

namespace ray::renderer_core_api
{

	bool RendererCommands::Initialize(IRenderer* renderer, PrimitiveTopology topology)
	{
		_device = renderer->_device;
		_classHelper = renderer->_classHelper;

		_3dCommandAllocator = _classHelper->CreateCommandAllocator();
		_commandList = _classHelper->CreateCommandList();
		_rootSignature = _classHelper->CreateRootSignature();
		_vertexShader = _classHelper->CreateVertexShader();
		_pixelShader = _classHelper->CreatePixelShader();
		_vertexBuffer = _classHelper->CreateVertexBuffer();
		_indexBuffer = _classHelper->CreateIndexBuffer();
		_resourceBarrier = _classHelper->CreateResourceBarrier();

		RootSignatureDesc rootSignatureDesc;
		rootSignatureDesc._num_params = 0;
		rootSignatureDesc._num_static_samplers = 0;
		
		if (!_device->CreateRootSignature(rootSignatureDesc, _rootSignature))
			return false;

#ifdef RAY_DEBUG
		_vertexShader->Compile("..\\shaders_d3d12\\VertexShader.hlsl");
		_pixelShader->Compile("..\\shaders_d3d12\\PixelShader.hlsl");
#else
		_vertex_shader->LoadBytecode("");
		_pixel_shader->LoadBytecode("");
#endif

		InputElementDesc inputLayout[] =
		{
			{ "POSITION", resources::ShaderType::eFloat3 }
		};

		PipelineStateDesc pipelineStateDesc;
		pipelineStateDesc._input_layout = inputLayout;
		pipelineStateDesc._input_layout_size = sizeof(inputLayout) / sizeof(InputElementDesc);
		pipelineStateDesc._num_render_targets = 1;
		pipelineStateDesc._vs = _vertexShader;
		pipelineStateDesc._ps = _pixelShader;
		pipelineStateDesc._root_signature = _rootSignature;
		pipelineStateDesc._topology = topology;

		if (!_device->CreatePipelineState(pipelineStateDesc, _pipelineState))
			return false;

		if (!_device->CreateCommandAllocator(_3dCommandAllocator, CommandListType::eDirect))
			return false;

		if (!_device->CreateCommandList(_commandList, _3dCommandAllocator, nullptr, CommandListType::eDirect))
			return false;

		return true;
	}


	void RendererCommands::Draw(Vertex* vertices, size_t vertexCount)
	{
		size_t bufferSize = vertexCount * Vertex::GetSize();

		resources::ResourceDesc desc;
		desc._width = bufferSize;
		if (!_device->CreateCommittedResource(desc, resources::ResourceUsage::eDefault, _vertexBuffer))
			return;
		_vertexBuffer->SetName("Vertex Buffer Default Heap");
		
		resources::IVertexBuffer* vertexBufferUpload = _classHelper->CreateVertexBuffer();
		if (!_device->CreateCommittedResource(desc, resources::ResourceUsage::eUpload, vertexBufferUpload))
			return;

		vertexBufferUpload->SetName("Vertex Buffer Upload Heap");

		vertexBufferUpload->SetData(static_cast<void*>(vertices), bufferSize);
		_commandList->UpdateSubresource(_vertexBuffer, vertexBufferUpload, 0, 0, 1);

		_resourceBarrier->Transition(_vertexBuffer, resources::ResourceState::eCopyDest, resources::ResourceState::eVertexAndConstantBuffer);
		_commandList->ResourceBarrier(_resourceBarrier, 1);
		_commandList->Close();
		// TODO: Resources must be copied to GPU memory
		_vertexBuffer->SetStride(u32(Vertex::GetSize()));
	}

	void RendererCommands::DrawIndexed(Vertex* vertices, size_t vertexCount, u32* indices, size_t indexCount)
	{

	}
}