#pragma once
#include "resources/resources.hpp"
#include "root_signature.hpp"
#include "vertex_shader.hpp"
#include "pixel_shader.hpp"

namespace ray::renderer_core_api
{
	struct InputElementDesc
	{
		InputElementDesc(pcstr name, resources::ShaderType type)
			: _semantic_name(name)
			, _type(type)
		{}

		InputElementDesc(const InputElementDesc&) = delete;
		InputElementDesc(InputElementDesc&&) = delete;

		pcstr _semantic_name;
		resources::ShaderType _type;
	};

	enum class PrimitiveTopology
	{
		eUndefined, //can't draw anything
		ePoint,
		eLine,
		eTriangle,
		ePatch //for tesselation
	};

	struct PipelineStateDesc
	{
		IRootSignature* _root_signature;
		IVertexShader* _vs;
		IPixelShader* _ps;
		resources::ShaderType _type;
		resources::ShaderType _rtv_types[8];
		PrimitiveTopology _topology;
		InputElementDesc* _input_layout;
		size_t _input_layout_size;
		u32 _num_render_targets;
	};

	/*
	**	Vulkan:			VkPipeline
	**	Direct3D 12:	ID3D12PipelineState
	*/
	class IPipelineState : public IRRCBase
	{
	public:
		virtual ~IPipelineState() {}

	};

}