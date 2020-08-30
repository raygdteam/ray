#include <editor/renderer/shader.hpp>
#include <core/file_system/file_system.hpp>
#include <engine/state/state.hpp>

VkShaderModule Shader::LoadShader(VkDevice device, pcstr path)
{
	IFile* shader = RayState()->FileSystem->OpenFile(path, ReadBinary);

	u8* buffer = new u8[shader->Size()];
	shader->Read(buffer, shader->Size());

	VkShaderModuleCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.codeSize = shader->Size(),
		.pCode = reinterpret_cast<u32*>(buffer)
	};

	VkShaderModule module = nullptr;
	vkCreateShaderModule(device, &createInfo, nullptr, &module);
	
	shader->Close();
	delete shader;

	return module;
}

