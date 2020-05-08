#pragma once

namespace ray::renderer::resources
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
	
class IResource
{
	//TODO
};
}