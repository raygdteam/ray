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

/* ------------------------ LEVEL ----------------------- */

void RLevel::Serialize(Archive&)
{	
}

void RLevel::Deserialize(Archive&)
{
}

bool RLevel::LoadFrom(IFile* path)
{
	//String data = {};
	//data.resize(path->Size());
	//path->Read((u8*)data.data(), path->Size());

	//nlohmann::json file = data.AsRawStr();
	//file["name"]
	return false;
}

ResourceType RLevel::GetResourceType() const noexcept
{
	return eLevel;
}

RAYOBJECT_DESCRIPTION_BEGIN(RTexture)
RAYOBJECT_DESCRIPTION_NAME("engine://resources/Texture")
RAYOBJECT_DESCRIPTION_END(RTexture);


/* ------------------ RESOURCE MANAGER ------------------ */

struct ResourceData
{
	String Name;
	u16 ReferenceCount;
	ResourceType Type;
	IRResource* ResourceRef;
};

struct ResourceMapping
{
	String Path;
	String Mapping;
	bool IsEngineCoreResources;
};

ResourceManager::ResourceManager(IRayState* state)
{
	_state = state;
	gDbgLog = new Logger("ResourceManager");
	
	_mapping.PushBack(ResourceMapping { 
		.Path = String("../../engine/resources"),
		.Mapping = String("/engine/"),
		.IsEngineCoreResources = true
	});
	_dataCacheDirectory = String("../../engine/datacache");
	//LoadResourceSync("/engine/tex.png", eTexture);
}

IRResource* ResourceManager::LoadResourceResolved(pcstr path, pcstr resorcePath, ResourceType type)
{
	check(type == eTexture);

	/* check in data cache */
	/*{
		String dcPath = _dataCacheDirectory;
		dcPath += "/";
		
		char crc32[32] = {};
		sprintf_s(crc32, "%u", Crc32((u8*)resorcePath, strlen(resorcePath)));
		dcPath.append(crc32);
		dcPath.append(".bundle");
		
		IFile* dcFile = _state->FileSystem->OpenFile(dcPath.c_str(), ReadBinary);

		if (dcFile != nullptr)
		{
			FileArchive ar;
			ar.file = dcFile;

			RTexture* texture = new RTexture;

			texture->Deserialize(ar);
			
			dcFile->Close();
			delete dcFile;
			return texture;
		}
	}*/
	
	IFile* file = gFileSystem.OpenFile(path, eReadBinary);
	check(file != nullptr);
	
	RTexture* texture = new RTexture;
	if (!texture->LoadFrom(file))
	{
		delete texture;

		file->Close();
		delete file;

		return nullptr;
	}
	
	file->Close();
	delete file;

	/* Serialize into data cache */
	/*String dcPath = _dataCacheDirectory;
	dcPath += "/";
	char crc32[32] = {};
	sprintf_s(crc32, "%u", Crc32((u8*)resorcePath, strlen(resorcePath)));
	dcPath.append(crc32);
	dcPath.append(".bundle");
	IFile* dcFile = _state->FileSystem->OpenFile(dcPath.c_str(), WriteBinary);
	FileArchive ar;
	ar.file = dcFile;

	texture->Serialize(ar);
	
	dcFile->Close();
	delete dcFile;*/
	
	return texture;
}

void ResourceManager::SetResourceDirectory(pcstr directory, pcstr mapping)
{
	/* BUG: assuming no dublicates */
	/* BUG: assuming no concurrency */
	
	if (mapping != nullptr)
	{
		_mapping.PushBack(ResourceMapping { .Path = String(directory), .Mapping = String(mapping), .IsEngineCoreResources = false });
		return;
	}

	String path = {};
	path += "resource_info.ray";
	
	IFile* info = gFileSystem.OpenFile(path.c_str(), eRead);
	ray_assert(info != nullptr, "Invalid mapping");
	ray_assert(info->Size() != 0, "Invalid mapping");

	String mappingPath;
	u64 size = info->Size() / sizeof(char);
	mappingPath.resize(size);

	// info->Read((void*)mappingPath.c_str(), size);
	
	info->Close();
	delete info;

	_mapping.PushBack(ResourceMapping { .Path = String(directory), .Mapping = String(mapping), .IsEngineCoreResources = false });
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

	for (ResourceMapping& resourceMapping : _mapping)
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

