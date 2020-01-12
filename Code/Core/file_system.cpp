#include "pch.hpp"

#include "file_system.hpp"
#include <filesystem>
#include <spdlog/spdlog.hpp>
#include <map>
#include "fs external lib/physfs.h"

class temp_fs
{
	std::map<pcstr, pcstr> _data;
public:
	pcstr read_file(pcstr path) { return _data[path]; }
	void write(pcstr path, pcstr contents) { _data[path] = contents; }
};

class file_system_impl
{
	temp_fs _temp_fs;
	// virtual path, real path
	std::map<pcstr, pcstr> _mounts;
public:
	void initialize();
	void query_mount(pcstr path, pcstr virtual_path);
	pcstr read_file(pcstr path);
	uint8_t* const read_file_bin(pcstr path);
	void write_file(pcstr path, pcstr contents);
	s64 file_size(pcstr path);
};

void file_system_impl::initialize()
{
	spdlog::info("file_system: working directory: {}",
		std::filesystem::current_path().generic_string().c_str());
	PHYSFS_init(nullptr);

	spdlog::info("file_system: initialization completed.");
}

void file_system_impl::query_mount(pcstr path, pcstr virtual_path)
{
	bool isDir = std::filesystem::is_directory(path);
	auto currentPath = std::filesystem::current_path();

	RAY_ASSERT(isDir, "проверьте рабочую директорию")

	PHYSFS_mount((currentPath / path).generic_string().c_str(), virtual_path, 1);
	spdlog::info("file_system: mounted '{}' to '{}'", path, virtual_path);
}

pcstr file_system_impl::read_file(pcstr path)
{
	PHYSFS_File* handle = PHYSFS_openRead(path);

	if (handle == nullptr)
	{
		spdlog::error("file_system: path '{}' not found", path);
		return nullptr;
	}

	auto size{ PHYSFS_fileLength(handle) };
	char* buf = static_cast<char*>(malloc(sizeof(char) * (size + 1)));
	memset(buf, 0, sizeof(char) * (size + 1));
	PHYSFS_read(handle, buf, (PHYSFS_uint32)1, (PHYSFS_uint32)(size + 1));
	PHYSFS_close(handle);
	return buf;
}

u8* const file_system_impl::read_file_bin(pcstr path)
{
	PHYSFS_File* handle = PHYSFS_openRead(path);

	if (handle == nullptr)
	{
		spdlog::error("file_system: path '{}' not found", path);
		return nullptr;
	}

	auto size{ PHYSFS_fileLength(handle) };
	auto sizee{ size };
	u8* buf = reinterpret_cast<u8*>(malloc(sizee));
	memset(buf, 0, sizee);
	PHYSFS_read(handle, buf, (PHYSFS_uint32)sizeof(u8), (PHYSFS_uint32)size);
	PHYSFS_close(handle);
	return buf;
}

void file_system_impl::write_file(pcstr path, pcstr contents)
{
	PHYSFS_File* handle = PHYSFS_openWrite(path);
	PHYSFS_write(handle, contents, (PHYSFS_uint32)1, (PHYSFS_uint32)strlen(contents) - 1);
	PHYSFS_close(handle);
}

s64 file_system_impl::file_size(pcstr path)
{
	PHYSFS_File* handle = PHYSFS_openRead(path);
	auto size{ PHYSFS_fileLength(handle) };
	PHYSFS_close(handle);
	return (s64)size;
}

file_system_impl _file_system;

void ray::file_system::query_mount(pcstr path, pcstr virtual_path) { _file_system.query_mount(path, virtual_path); }

void ray::file_system::initialize() { _file_system.initialize(); }

pcstr ray::file_system::read_file(pcstr path) { return _file_system.read_file(path); }

void ray::file_system::write_file(pcstr path, pcstr contents) { _file_system.write_file(path, contents); }

uint8_t* ray::file_system::read_file_bin(pcstr path) { return _file_system.read_file_bin(path); }

s64 ray::file_system::file_size(pcstr path) { return _file_system.file_size(path); }
