#pragma once
#include "pch.hpp"
#include "core.h"
#include <vector>

namespace ray {

	class file_system
	{
	public:
		static void query_mount(pcstr real_path, pcstr virtual_path);
		static void initialize();
		static pcstr read_file(pcstr path);
		static void write_file(pcstr path, pcstr contents);
		static void write_file_bin(pcstr path, std::vector<u32> contents);
		static uint8_t* read_file_bin(pcstr path);
		static s64 file_size(pcstr path);
	};

}
