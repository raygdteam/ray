#pragma once

namespace ray::resources
{

enum eResourceType
{
	eNull,
	eRaw,

	/** Texture in Ray's file format. Raw pixels basically. */
	eTexture,

	eMaterial,

	eSound,
};

struct IResource
{
	eResourceType type;
	void* data; /* Memory is managed by ResourceManager. */
};

/* For use in Bundles. */
struct IResourceRef
{
	char path[260];
	eResourceType type;
};

}