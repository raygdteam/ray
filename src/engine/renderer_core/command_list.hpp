#pragma once

/*
**	Vulkan:			VkCommandBuffer
**	Direct3D 12:	ID3D12CommandList / ID3D12GraphicsCommandList
*/

namespace ray::renderer_core_api
{
enum class CommandListType
{
	direct,
	bundle,
	compute,
	copy
};

class ICommandList : public IRRCBase
{
public:
	virtual void Close() = 0;

};
}