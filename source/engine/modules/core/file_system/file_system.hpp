#pragma once

#include <string>

namespace ray::file_system
{
	void mount_directory(const char* virtual_directory, const char* physical_directory);
	void unmount_directory(const char* virtual_directory);

	bool file_exists(std::string file_path);
	bool directory_exists(std::string directory_path);
};