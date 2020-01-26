#pragma once
#include "pch.hpp"
#include "core.hpp"
#include "serialization/archive.hpp"

namespace ray 
{

class FileSystem
{
	FileSystem() = default;
public:
	static FileSystem Get();

	void initialize();
	void mount(pcstr real_path, pcstr virtual_path);
	serialization::IArchive ReadFile(pcstr path);
	void write_file(pcstr path, pcstr contents);
	//void write_file_bin(pcstr path, std::vector<u32> contents);
	uint8_t* read_file_bin(pcstr path);
	s64 file_size(pcstr path);
};

}
