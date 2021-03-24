#include <core/core.hpp>
#include <core/memory/new_delete_override.hpp>
#include <core/debug/assert.hpp>
#include <core/file_system/file_system.hpp>
#include <core/object/file_archive.hpp>
#include <core/log/log.hpp>
#include <core/extended_instuctions/sse/common.hpp>
#include <core/json/json.hpp>

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

	JsonValue json = JsonParser::Parse(text);
	check(json["ResourceType"].AsString() == String("Material"));

	return true;
}

ResourceType RMaterial::GetResourceType() const noexcept
{
	return eMaterial;
}

void RMaterial::Unload()
{
}

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

	JsonValue json = JsonParser::Parse(text);
	check(json["ResourceType"].AsString() == String("MaterialInstance"));

	Texture = json["Properties"]["TextureName"].AsString();

	return true;
}

ResourceType RMaterialInstance::GetResourceType() const noexcept
{
	return eMaterialInstance;
}

void RMaterialInstance::Unload()
{
}


/* ------------------ RESOURCE MANAGER ------------------ */

struct ResourceData
{
	String Name;
	u16 ReferenceCount;
	ResourceType Type;
	IRResource* ResourceRef;
};

ResourceManager::ResourceManager(IRayState* state)
{
	_state = state;
	gDbgLog = new Logger("ResourceManager");
	
	_dataCacheDirectory = String("../../engine/datacache");
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

	resource->_name = String(resorcePath);
	resource->LoadFrom(file);
	
	file->Close();
	delete file;

	return resource;
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
	for (ResourceData& resource : _resources)
	{
		resource.ResourceRef->Unload();
	}

	_resources.Clear();
}

IRResource* ResourceManager::LoadResourceSync(pcstr inName, ResourceType desiredType)
{
	check(inName != nullptr);
	check(inName[0] == '/');

	for (ResourceData& resource : _resources)
	{
		if (resource.Type == desiredType && resource.Name == inName)
		{
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
			_resources.PushBack(ResourceData {
				.Name = String(inName),
				.ReferenceCount = 0xffff,
				.Type = desiredType,
				.ResourceRef = resource
			});
			
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

