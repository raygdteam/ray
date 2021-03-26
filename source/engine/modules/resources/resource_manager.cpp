#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>
#include <core/debug/assert.hpp>
#include <core/file_system/file_system.hpp>
#include <core/object/file_archive.hpp>
#include <core/log/log.hpp>
#include <core/extended_instuctions/sse/common.hpp>

#include <resources/resource.hpp>
#include <resources/resource_manager.hpp>
#include <engine/state/state.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma clang diagnostic pop

static Logger* gDbgLog;

/* -------------------- RESOURCE BASE ------------------- */
RAYOBJECT_DESCRIPTION_BEGIN(IRResource)
RAYOBJECT_DESCRIPTION_NAME("engine://resources/IRResource")
RAYOBJECT_DESCRIPTION_END(IRResource);

static u64 gLastId = 0;

/* ----------------------- TEXTURE ---------------------- */

RTexture::RTexture()
{
	_id = gLastId++;
}

bool RTexture::LoadFrom(IFile* path)
{
	u32 size = u32(path->Size());
	u8* buffer = new u8[size];
	path->Seek(0, Begin);
	path->Read(buffer, size);

	s32 x = 0;
	s32 y = 0;
	s32 channels = 0;

	float* result = stbi_loadf_from_memory(buffer, size, &x, &y, &channels, 4);

	if (result == nullptr)
	{
		delete[] buffer;
		return false;
	}

	_dimensions.x = x;
	_dimensions.y = y;

	FVector4* vecResult = reinterpret_cast<FVector4*>(result);
	
	for (s32 i = 0; i < x * y; ++i)
	{
		_data.PushBack({.x = vecResult[i].x, .y = vecResult[i].y, .z = vecResult[i].z, .w = vecResult[i].w });
	}

	stbi_image_free(result);
	delete[] buffer;
	return true;
}

ResourceType RTexture::GetResourceType() const noexcept
{
	return ResourceType::eTexture;
}

const Array<FVector4>& RTexture::GetData() const
{
	return _data;
}

const FVector2& RTexture::GetDimensions() const
{
	return _dimensions;
}

void RTexture::Serialize(Archive& ar)
{
	ar.Write(_dimensions);
	ar.Write((void*)_data.GetData(), _data.Size() * sizeof(FVector4));
}

void RTexture::Deserialize(Archive& ar)
{
	ar.Read(_dimensions);
	u64 size = _dimensions.x * _dimensions.y;
	_data.Resize(size);
	ar.Read((u8*)_data.GetData(), size * sizeof(FVector4));
}

void RTexture::Unload()
{
	_data.Clear();
	_data.shrink_to_fit();
}

RAYOBJECT_DESCRIPTION_BEGIN(RTexture)
RAYOBJECT_DESCRIPTION_NAME("engine://resources/Texture")
RAYOBJECT_DESCRIPTION_END(RTexture);

/* ----------------------- MATERIAL ---------------------- */

void RMaterial::Serialize(Archive&)
{
}

void RMaterial::Deserialize(Archive&)
{
}

bool RMaterial::LoadFrom(IFile* file)
{
	String text;
	text.resize(file->Size());
	file->Read((u8*)text.data(), file->Size());

	ray::json::dictionary json = ray::json::parse(text.c_str());
	check(json["ResourceType"].as_string() == String("Material"));

	return true;
}

ResourceType RMaterial::GetResourceType() const noexcept
{
	return eMaterial;
}

void RMaterial::Unload()
{
}

RAYOBJECT_DESCRIPTION_BEGIN(RMaterial)
RAYOBJECT_DESCRIPTION_NAME("engine://resources/Material")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(RMaterial)

/* ----------------- MATERIAL INSTANCE ----------------- */

void RMaterialInstance::Serialize(Archive&)
{
}

void RMaterialInstance::Deserialize(Archive&)
{
}

bool RMaterialInstance::LoadFrom(IFile* file)
{
	String text;
	text.resize(file->Size());
	file->Read((u8*)text.data(), file->Size());

	ray::json::dictionary json = ray::json::parse(text.c_str());
	check(json["ResourceType"].as_string() == String("MaterialInstance"));

	Texture = String(json["Properties"].as_dictionary()["TextureName"].as_string());

	return true;
}

ResourceType RMaterialInstance::GetResourceType() const noexcept
{
	return eMaterialInstance;
}

void RMaterialInstance::Unload()
{
	Texture = String("");
}

RAYOBJECT_DESCRIPTION_BEGIN(RMaterialInstance)
RAYOBJECT_DESCRIPTION_NAME("engine://resources/MaterialInstance")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(RMaterialInstance)

/* --------------- CONFIGURATION -------------- */

void RConfiguration::Serialize(Archive&)
{
}

void RConfiguration::Deserialize(Archive&)
{
}

bool RConfiguration::LoadFrom(IFile* path)
{
	String text;
	text.resize(path->Size());
	path->Read((u8*)text.data(), path->Size());

	Root = ray::json::parse(text.c_str());
	return true;
}

ResourceType RConfiguration::GetResourceType() const noexcept
{
	return eConfig;
}

void RConfiguration::Unload()
{
	Root = ray::json::dictionary();
}

RAYOBJECT_DESCRIPTION_BEGIN(RConfiguration)
RAYOBJECT_DESCRIPTION_NAME("engine://resources/Configuration")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(RConfiguration)

/* ------------------ RESOURCE MANAGER ------------------ */

void SplitPath(String path, String& outName, Array<String>& out)
{
	if (*path.begin() == '/') 
		path = String(path.substr(1));

	outName = String(path.substr(path.find_last_of("/") + 1));
	path = String(path.substr(0, path.find_last_of("/")));
	
	if (path.find_first_of("/") != String::npos)
	{
		while (path.find_first_of("/") != String::npos)
		{
			size_t n = path.find_first_of("/");
			out.PushBack(String(path.substr(0, n)));
			path = String(path.substr(n + 1));
		}

		// there is only one left
		out.PushBack(path);
	}
	else
	{
		// rpath contains the only component
		if (!path.empty())
		{
			out.PushBack(path);
		}
	}
}

ResourceManager::ResourceManager(IRayState* state)
{
	_state = state;
	gDbgLog = new Logger("ResourceManager");
	
	_dataCacheDirectory = String("../../engine/datacache");
}

void ResourceManager::LoadPreloadedResource(ResourceData& data)
{
	IFile* file = gFileSystem.OpenFile(data.PhysicalPath.AsRawStr(), eReadBinary);
	check(file != nullptr);

	data.ResourceRef->LoadFrom(file);
	data.ResourceRef->_loaded = true;

	file->Close();
	delete file;
}

IRResource* ResourceManager::LoadResourceResolved(pcstr path, pcstr resorcePath, ResourceType type)
{
	// TODO: kostil
	String realPath(path);
	if (type != eTexture) realPath.append(".json");
	
	IFile* file = gFileSystem.OpenFile(realPath.AsRawStr(), eReadBinary);
	check(file != nullptr);

	IRResource* resource = nullptr;
	
	if (type == eTexture)
	{
		resource = new RTexture();
	}
	else if (type == eMaterial)
	{
		resource = new RMaterial();
	}
	else if (type == eMaterialInstance)
	{
		resource = new RMaterialInstance();
	}
	else if (type == eConfig)
	{
		resource = new RConfiguration();
	}

	resource->_name = String(resorcePath);
	resource->LoadFrom(file);
	resource->_loaded = true;
	
	file->Close();
	delete file;

	return resource;
}

void ResourceManager::PreloadResource(String& path, String& resourcePath)
{
	String extension(path.substr(path.find_last_of(".") + 1));
	IRResource* res = nullptr;
	
	if (extension == "ray_level")
	{
		return;
	}
	else if(extension == "ray_material")
	{
		resourcePath = String(resourcePath.substr(0, resourcePath.find_last_of(".")));
		gDbgLog->Log("PreloadResource(): Loading '%s' as RMaterial", resourcePath.AsRawStr());
		res = new RMaterial();
	}
	else if (extension == "ray_materialinstance")
	{
		resourcePath = String(resourcePath.substr(0, resourcePath.find_last_of(".")));
		gDbgLog->Log("PreloadResource(): Loading '%s' as RMaterialInstance", resourcePath.AsRawStr());
		res = new RMaterialInstance();
	}
	else if(extension == "ray_config")
	{
		resourcePath = String(resourcePath.substr(0, resourcePath.find_last_of(".")));
		gDbgLog->Log("PreloadResource(): Loading '%s' as RConfiguration", resourcePath.AsRawStr());
		res = new RConfiguration();
	}
	else
	{
		gDbgLog->Log("PreloadResource(): Loading '%s' as RTexture", resourcePath.AsRawStr());
		res = new RTexture();
	}

	res->_name = resourcePath;

	String resName;
	Array<String> resPathComponents;
	SplitPath(resourcePath, resName, resPathComponents);

	InsertResource(ResourceData {
		.Name = resourcePath,
		.PhysicalPath = path,
		.ReferenceCount = 0xffff,
		.Type = res->GetResourceType(),
		.ResourceRef = res
	}, resName, resPathComponents);
}

void ResourceManager::InsertResource(ResourceData data, String& name, Array<String>& components)
{
	Directory* dir = &_rootDirectory.Directories[components[0]];
	bool bFirst = true;

	for (String& pathComponent : components)
	{
		if (bFirst)
		{
			dir = &_rootDirectory.Directories[pathComponent];
			bFirst = false;
		}
		else
		{
			dir = &dir->Directories[pathComponent];
		}
	}

	dir->Resources[name] = data;
}

Directory& ResourceManager::GetDirectory(Array<String>& components)
{
	Directory* dir = &_rootDirectory.Directories[components[0]];
	bool bFirst = true;

	for (String& pathComponent : components)
	{
		if (bFirst)
		{
			dir = &_rootDirectory.Directories[pathComponent];
			bFirst = false;
		}
		else
		{
			dir = &dir->Directories[pathComponent];
		}
	}

	return *dir;
}

void ResourceManager::UnloadResourcesInternal(Directory& dir)
{
	for (eastl::pair<const String, ResourceData>& pair : dir.Resources)
	{
		pair.second.ResourceRef->Unload();
	}

	for (eastl::pair<const String, Directory>& directory : dir.Directories)
	{
		UnloadResourcesInternal(directory.second);
	}
}

void ResourceManager::SetEngineResourcesDirectory(String& directory)
{
	_engineMap.Path = directory;
	_engineMap.Mapping = String("/engine/");
}

void ResourceManager::SetGameResourceDirectory(String& directory)
{
	_gameMap.Path = directory;
	_gameMap.Mapping = String("/game/");
}

void ResourceManager::UnloadAllResources()
{
	UnloadResourcesInternal(_rootDirectory);
}

IRResource* ResourceManager::LoadResourceSync(pcstr inName, ResourceType desiredType)
{
	check(inName != nullptr);
	check(inName[0] == '/');


	String resName;
	Array<String> resPathComponents;
	SplitPath(String(inName), resName, resPathComponents);

	for (eastl::pair<const String, ResourceData>& pair : GetDirectory(resPathComponents).Resources)
	{
		ResourceData& resource = pair.second;
		
		if (resource.Type == desiredType && resource.Name == inName)
		{
			if (!resource.ResourceRef->IsLoaded())
			{
				gDbgLog->Log("Loading %s", inName);
				LoadPreloadedResource(resource);
			}
			return resource.ResourceRef;
		}
	}

	gDbgLog->Log("Loading %s", inName);

	u32 mappingEnd = strchr(inName + 1, '/') - inName;
	check(mappingEnd != 0);

	String mapping(inName, 0, mappingEnd + 1);

	_mutex.Enter();

	ResourceMapping mappings[2] = {_engineMap, _gameMap};

	for (ResourceMapping& resourceMapping : mappings)
	{
		if (resourceMapping.Mapping == mapping)
		{
			String resolvedPath(resourceMapping.Path);
			resolvedPath += (inName + mappingEnd);
			
			IRResource* resource = LoadResourceResolved(resolvedPath.c_str(), inName, desiredType);

			if (resource == nullptr)
			{
				/* shortcut */
				_mutex.Leave();
				check(false);
			}

			/* shortcut */
			InsertResource(ResourceData {
				.Name = String(inName),
				.PhysicalPath = resolvedPath,
				.ReferenceCount = 0xffff,
				.Type = desiredType,
				.ResourceRef = resource
			}, resName, resPathComponents);
			
			_mutex.Leave();
			return resource;
		}
	}
	
	_mutex.Leave();
	return nullptr;
}

void ResourceManager::LoadResource(pcstr inName, ResourceType desiredType, Function<void(IRResource*)> callback)
{
	(void)inName;
	(void)desiredType;
	(void)callback;
	
	/* BUG: assuming no concurrency */

	//check(inName != nullptr);
	//check(inName[0] == '/');

	//u32 mappingEnd = strchr(inName + 1, '/') - inName;
	//check(mappingEnd != 0);
	//check(mappingEnd <= 16);

	//char mappingRaw[17] = {};
	//for (u8 i = 0; i < mappingEnd; ++i)
	//{
	//	mappingRaw[mappingEnd] = inName[mappingEnd];
	//}

	//mappingRaw[mappingEnd + 1] = '\0';

	//String mapping(mappingRaw);
	//gDbgLog.Log("Mapping {}", mapping.c_str());

	//for (ResourceMapping& resourceMapping : _mapping)
	//{
	//	if (resourceMapping.Mapping == mapping)
	//	{
	//		String resolvedPath(resourceMapping.Path);
	//		resolvedPath += (inName + mappingEnd);
	//		IRResource* resource = LoadResourceResolved(resolvedPath.c_str(), desiredType);

	//		if (resource == nullptr)
	//		{
	//			/* shortcut */
	//			check(false);
	//		}

	//		/* shortcut */
	//		_resources.PushBack(ResourceData {
	//			.Name = String(inName),
	//			.ReferenceCount = 0xffff,
	//			.Type = desiredType,
	//			.ResourceRef = resource
	//		});

	//		callback(resource);
	//	}
	//}
}

void ResourceManager::PreloadAllEngineResources()
{
	Array<String> files;
	gFileSystem.ListFilesRecursively(_engineMap.Path, files);

	for (String& file : files)
	{
		String resPath = file;
		resPath.replace(0, _engineMap.Path.Length() + 1, _engineMap.Mapping);
		PreloadResource(file, resPath);
	}
}

void ResourceManager::PreloadAllGameResources()
{
	Array<String> files;
	gFileSystem.ListFilesRecursively(_gameMap.Path, files);

	for (String& file : files)
	{
		String resPath = file;
		resPath.replace(0, _gameMap.Path.Length() + 1, _gameMap.Mapping);
		PreloadResource(file, resPath);
	}
}

