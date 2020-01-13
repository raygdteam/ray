#include "pch.hpp"
#include "ShaderModule.hpp"

namespace ray::renderer::vulkan::classes::shaders
{

const TBuiltInResource DefaultTBuiltInResource = {
	/* .MaxLights = */ 32,
	/* .MaxClipPlanes = */ 6,
	/* .MaxTextureUnits = */ 32,
	/* .MaxTextureCoords = */ 32,
	/* .MaxVertexAttribs = */ 64,
	/* .MaxVertexUniformComponents = */ 4096,
	/* .MaxVaryingFloats = */ 64,
	/* .MaxVertexTextureImageUnits = */ 32,
	/* .MaxCombinedTextureImageUnits = */ 80,
	/* .MaxTextureImageUnits = */ 32,
	/* .MaxFragmentUniformComponents = */ 4096,
	/* .MaxDrawBuffers = */ 32,
	/* .MaxVertexUniformVectors = */ 128,
	/* .MaxVaryingVectors = */ 8,
	/* .MaxFragmentUniformVectors = */ 16,
	/* .MaxVertexOutputVectors = */ 16,
	/* .MaxFragmentInputVectors = */ 15,
	/* .MinProgramTexelOffset = */ -8,
	/* .MaxProgramTexelOffset = */ 7,
	/* .MaxClipDistances = */ 8,
	/* .MaxComputeWorkGroupCountX = */ 65535,
	/* .MaxComputeWorkGroupCountY = */ 65535,
	/* .MaxComputeWorkGroupCountZ = */ 65535,
	/* .MaxComputeWorkGroupSizeX = */ 1024,
	/* .MaxComputeWorkGroupSizeY = */ 1024,
	/* .MaxComputeWorkGroupSizeZ = */ 64,
	/* .MaxComputeUniformComponents = */ 1024,
	/* .MaxComputeTextureImageUnits = */ 16,
	/* .MaxComputeImageUniforms = */ 8,
	/* .MaxComputeAtomicCounters = */ 8,
	/* .MaxComputeAtomicCounterBuffers = */ 1,
	/* .MaxVaryingComponents = */ 60,
	/* .MaxVertexOutputComponents = */ 64,
	/* .MaxGeometryInputComponents = */ 64,
	/* .MaxGeometryOutputComponents = */ 128,
	/* .MaxFragmentInputComponents = */ 128,
	/* .MaxImageUnits = */ 8,
	/* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
	/* .MaxCombinedShaderOutputResources = */ 8,
	/* .MaxImageSamples = */ 0,
	/* .MaxVertexImageUniforms = */ 0,
	/* .MaxTessControlImageUniforms = */ 0,
	/* .MaxTessEvaluationImageUniforms = */ 0,
	/* .MaxGeometryImageUniforms = */ 0,
	/* .MaxFragmentImageUniforms = */ 8,
	/* .MaxCombinedImageUniforms = */ 8,
	/* .MaxGeometryTextureImageUnits = */ 16,
	/* .MaxGeometryOutputVertices = */ 256,
	/* .MaxGeometryTotalOutputComponents = */ 1024,
	/* .MaxGeometryUniformComponents = */ 1024,
	/* .MaxGeometryVaryingComponents = */ 64,
	/* .MaxTessControlInputComponents = */ 128,
	/* .MaxTessControlOutputComponents = */ 128,
	/* .MaxTessControlTextureImageUnits = */ 16,
	/* .MaxTessControlUniformComponents = */ 1024,
	/* .MaxTessControlTotalOutputComponents = */ 4096,
	/* .MaxTessEvaluationInputComponents = */ 128,
	/* .MaxTessEvaluationOutputComponents = */ 128,
	/* .MaxTessEvaluationTextureImageUnits = */ 16,
	/* .MaxTessEvaluationUniformComponents = */ 1024,
	/* .MaxTessPatchComponents = */ 120,
	/* .MaxPatchVertices = */ 32,
	/* .MaxTessGenLevel = */ 64,
	/* .MaxViewports = */ 16,
	/* .MaxVertexAtomicCounters = */ 0,
	/* .MaxTessControlAtomicCounters = */ 0,
	/* .MaxTessEvaluationAtomicCounters = */ 0,
	/* .MaxGeometryAtomicCounters = */ 0,
	/* .MaxFragmentAtomicCounters = */ 8,
	/* .MaxCombinedAtomicCounters = */ 8,
	/* .MaxAtomicCounterBindings = */ 1,
	/* .MaxVertexAtomicCounterBuffers = */ 0,
	/* .MaxTessControlAtomicCounterBuffers = */ 0,
	/* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
	/* .MaxGeometryAtomicCounterBuffers = */ 0,
	/* .MaxFragmentAtomicCounterBuffers = */ 1,
	/* .MaxCombinedAtomicCounterBuffers = */ 1,
	/* .MaxAtomicCounterBufferSize = */ 16384,
	/* .MaxTransformFeedbackBuffers = */ 4,
	/* .MaxTransformFeedbackInterleavedComponents = */ 64,
	/* .MaxCullDistances = */ 8,
	/* .MaxCombinedClipAndCullDistances = */ 8,
	/* .MaxSamples = */ 4,
	/* .maxMeshOutputVerticesNV = */ 256,
	/* .maxMeshOutputPrimitivesNV = */ 512,
	/* .maxMeshWorkGroupSizeX_NV = */ 32,
	/* .maxMeshWorkGroupSizeY_NV = */ 1,
	/* .maxMeshWorkGroupSizeZ_NV = */ 1,
	/* .maxTaskWorkGroupSizeX_NV = */ 32,
	/* .maxTaskWorkGroupSizeY_NV = */ 1,
	/* .maxTaskWorkGroupSizeZ_NV = */ 1,
	/* .maxMeshViewCountNV = */ 4,

	/* .limits = */ {
	/* .nonInductiveForLoops = */ 1,
	/* .whileLoops = */ 1,
	/* .doWhileLoops = */ 1,
	/* .generalUniformIndexing = */ 1,
	/* .generalAttributeMatrixVectorIndexing = */ 1,
	/* .generalVaryingIndexing = */ 1,
	/* .generalSamplerIndexing = */ 1,
	/* .generalVariableIndexing = */ 1,
	/* .generalConstantMatrixVectorIndexing = */ 1,
} };

EShLanguage translateShaderStage(vk::ShaderStageFlagBits stage)
{
	switch (stage)
	{
	case vk::ShaderStageFlagBits::eVertex:                  return EShLangVertex;
	case vk::ShaderStageFlagBits::eTessellationControl:     return EShLangTessControl;
	case vk::ShaderStageFlagBits::eTessellationEvaluation:  return EShLangTessEvaluation;
	case vk::ShaderStageFlagBits::eGeometry:                return EShLangGeometry;
	case vk::ShaderStageFlagBits::eFragment:                return EShLangFragment;
	case vk::ShaderStageFlagBits::eCompute:                 return EShLangCompute;
	case vk::ShaderStageFlagBits::eRaygenNV:                return EShLangRayGenNV;
	case vk::ShaderStageFlagBits::eAnyHitNV:                return EShLangAnyHitNV;
	case vk::ShaderStageFlagBits::eClosestHitNV:            return EShLangClosestHitNV;
	case vk::ShaderStageFlagBits::eMissNV:                  return EShLangMissNV;
	case vk::ShaderStageFlagBits::eIntersectionNV:          return EShLangIntersectNV;
	case vk::ShaderStageFlagBits::eCallableNV:              return EShLangCallableNV;
	case vk::ShaderStageFlagBits::eTaskNV:                  return EShLangTaskNV;
	case vk::ShaderStageFlagBits::eMeshNV:                  return EShLangMeshNV;
	default:
		assert(false && "Unknown shader stage");
		return EShLangVertex;
	}
}

void ShaderModule::Initialize(vk::UniqueDevice const& device, pcstr path, vk::ShaderStageFlagBits shaderFlags)
{
	// Try to find shader in cache
	std::string _path = ("/local/cache/vulkan" + std::string(path) + ".built");
	/*u8* data = file_system::read_file_bin(_path);
	if (data != nullptr)
	{
		u32 size = file_system::file_size(_path);
		vk::ShaderModuleCreateInfo shaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), size * sizeof(u8), (u32*)data);
		_shader = device->createShaderModuleUnique(shaderModuleCreateInfo);
		spdlog::debug("vulkanrenderer: hit shader {} in cache", path);
		return;
	}*/
	std::string src(file_system::read_file(path));
	std::vector<u32> res;

	// Compile shader
	glslang::InitializeProcess();
	EShLanguage stage = translateShaderStage(shaderFlags);

	const char* shaderStrings[1];
	shaderStrings[0] = src.data();

	glslang::TShader shader(stage);
	shader.setStrings(shaderStrings, 1);

	// Enable SPIR-V and Vulkan rules when parsing GLSL
	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

	if (!shader.parse(&DefaultTBuiltInResource, 100, false, messages))
	{
		spdlog::trace(shader.getInfoLog());
		spdlog::trace(shader.getInfoDebugLog());
		return;  // something didn't work
	}

	glslang::TProgram program;
	program.addShader(&shader);

	//
	// Program-level processing...
	//

	if (!program.link(messages))
	{
		spdlog::trace(shader.getInfoLog());
		spdlog::trace(shader.getInfoDebugLog());
		return;
	}

	glslang::GlslangToSpv(*program.getIntermediate(stage), res);

	glslang::FinalizeProcess();
	spdlog::debug("vulkanrenderer: built shader {}", path);

	//file_system::write_file_bin(_path.c_str(), res);
	
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), res.size() * sizeof(u32), (u32*)res.data());
	_shader = device->createShaderModuleUnique(shaderModuleCreateInfo);
}
}
