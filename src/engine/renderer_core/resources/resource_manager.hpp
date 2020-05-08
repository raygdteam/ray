#pragma once

#include "buffer.hpp"
#include "textures.hpp"

namespace ray::renderer::resources
{
class ResourceManager
{
public:
	//TODO
	static Buffer* CreateBuffer(void* buffer, BufferDesc& desc);
	static Texture1D* CreateTexture1D();
	static Texture2D* CreateTexture2D();
	static Texture3D* CreateTexture3D();
	
};
}