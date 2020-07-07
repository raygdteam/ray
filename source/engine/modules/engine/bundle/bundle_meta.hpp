#pragma once
#include <core/core.hpp>

struct BundleHeader
{
	u64 Magic; /* bin'RAY!' */
	u8 Datatype;
	u64 Checksum;
};

struct Bundle
{
	BundleHeader Header;
};
