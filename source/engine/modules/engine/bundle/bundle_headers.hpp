#pragma once
#include <ray/type/extensions.hpp>

namespace ray::bundle
{

struct BundleHeaderSection
{
	u32 Id;

};

struct BundleHeader
{
	u64 Magic; // 0x32, 0x64, 0x6A, 0x9B, 0xFA, 0x45
	u32 Version; // (optional) (x, y, z) => x * 10000 + y * 100 + z | IMPORTANT!!! 0 < x,y,z < 10
	u32 Checksum;
	u32 FilesSize;
};

struct Bundle
{
	BundleHeader Header;

	unsigned char data[];
};

}