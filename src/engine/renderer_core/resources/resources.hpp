#pragma once
#include "../ray_renderer_core_base.hpp"

namespace ray::renderer_core_api::resources
{

enum class Usage
{
	//gpu can read and write
	usage_default,
	//cpu can read and gpu can write
	usage_dynamic,
	//gpu can read
	usage_immutable
};

enum class Flags
{
	vertex_buffer,
	index_buffer,
	constant_buffer
};

enum class ResourceState
{
	present,
	render_target
};
	
class IResource : public ray::renderer_core_api::IRRCBase
{
public:
	virtual ~IResource() {}
	//TODO
};


class IResourceBarrier : public ray::renderer_core_api::IRRCBase
{
public:
	virtual ~IResourceBarrier() {}
	virtual void Transition(IResource*, ResourceState, ResourceState) = 0;
};

}