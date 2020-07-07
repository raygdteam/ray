#include "renderer_commands.hpp"
#include "vertex_shader.hpp"
#include "pixel_shader.hpp"

namespace ray::renderer_core_api
{
	std::vector<ICommandList*> RendererCommands::_lists = {};
	ICommandQueue* RendererCommands::_command_queue = nullptr;

	bool RendererCommands::Initialize(IRenderer* renderer, PrimitiveTopology topology)
	{
		_device = renderer->_device;
		_class_helper = renderer->_class_helper;
		static bool sFirst = true;
		if (sFirst)
		{
			_command_queue = renderer->_command_queue;
			sFirst = false;
		}

		_command_list = _class_helper->CreateCommandList();
		_root_signature = _class_helper->CreateRootSignature();
		_vertex_shader = _class_helper->CreateVertexShader();
		_pixel_shader = _class_helper->CreatePixelShader();
		_vertex_buffer = _class_helper->CreateVertexBuffer();
		_index_buffer = _class_helper->CreateIndexBuffer();
		_resource_barrier = _class_helper->CreateResourceBarrier();

		RootSignatureDesc rootSignatureDesc;
		rootSignatureDesc._num_params = 0;
		rootSignatureDesc._num_static_samplers = 0;
		
		if (!_device->CreateRootSignature(rootSignatureDesc, _root_signature))
			return false;

#ifdef RAY_DEBUG
		_vertex_shader->Compile("..\\shaders_d3d12\\VertexShader.hlsl");
		_pixel_shader->Compile("..\\shaders_d3d12\\PixelShader.hlsl");
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
		pipelineStateDesc._vs = _vertex_shader;
		pipelineStateDesc._ps = _pixel_shader;
		pipelineStateDesc._root_signature = _root_signature;
		pipelineStateDesc._topology = topology;

		if (!_device->CreatePipelineState(pipelineStateDesc, _pipeline_state))
			return false;

		if (!_device->CreateCommandList(_command_list, renderer->_command_allocators[0], nullptr, CommandListType::eDirect))
			return false;

		_lists.push_back(_command_list);

		return true;
	}

	void RendererCommands::BeginScene()
	{

	}

	void RendererCommands::EndScene()
	{

	}

	void RendererCommands::Execute()
	{
		_command_queue->SetCommandLists(_lists.data(), _lists.size());
		_command_queue->ExecuteCommandLists();
	}

	void RendererCommands::Draw(Vertex* vertices, size_t vertexCount)
	{
		size_t bufferSize = vertexCount * Vertex::GetSize();

		resources::ResourceDesc desc;
		desc._width = bufferSize;
		if (!_device->CreateCommittedResource(desc, resources::ResourceUsage::eDefault, _vertex_buffer))
			return;
		_vertex_buffer->SetName("Vertex Buffer Default Heap");
		
		resources::IVertexBuffer* vertexBufferUpload = _class_helper->CreateVertexBuffer();
		if (!_device->CreateCommittedResource(desc, resources::ResourceUsage::eUpload, vertexBufferUpload))
			return;

		vertexBufferUpload->SetName("Vertex Buffer Upload Heap");

		vertexBufferUpload->SetData(static_cast<void*>(vertices), bufferSize);
		_command_list->UpdateSubresource(_vertex_buffer, vertexBufferUpload, 0, 0, 1);

		_resource_barrier->Transition(_vertex_buffer, resources::ResourceState::eCopyDest, resources::ResourceState::eVertexAndConstantBuffer);
		_command_list->ResourceBarrier(_resource_barrier, 1);
		_command_list->Close();

		_command_queue->SetCommandLists(&_command_list, 1);
		_command_queue->ExecuteCommandLists();

		_vertex_buffer->SetStride(Vertex::GetSize());
	}

	void RendererCommands::DrawIndexed(Vertex* vertices, size_t vertexCount, u32* indices, size_t indexCount)
	{

	}
}