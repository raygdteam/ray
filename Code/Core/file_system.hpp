#pragma once
#include "pch.hpp"
#include "core.h"

namespace ray {

	class file_system
	{
	public:
		static void query_mount(pcstr, pcstr);
		static void initialize();
		static pcstr read_file(pcstr path);
		static void write_file(pcstr path, pcstr contents);
		static uint8_t* read_file_bin(pcstr path);
		static s64 file_size(pcstr path);
	};

}
