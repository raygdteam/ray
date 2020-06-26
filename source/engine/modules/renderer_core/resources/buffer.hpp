#pragma once

#include "resources.hpp"

namespace ray::renderer_core_api::resources
{

struct BufferDesc
{
	size_t size;
	size_t byte_stride;
	Usage usage;
	Flags flags;
};
	
class Buffer : public IResource
{
	//TODO
private:
	void* _buffer;
};
	
}

