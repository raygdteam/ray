#pragma once
#include "core.hpp"

namespace ray::core::platform
{

enum class Feature : u32
{
	SSE = BIT(0),
	SSE2 = BIT(1),
	SSE3 = BIT(2),
	SSSE3 = BIT(3),
	SSE4_1 = BIT(4),
	SSE4_2 = BIT(5),
	SSE4A = BIT(6),
	SSE5 = BIT(7),
	AVX = BIT(8)
};

class PlatformProcessor
{
public:
	static std::string GetModelName();
	static u16 GetCacheLineSize();
	static bool HasFeature(Feature);
	static u32 GetCoreCount();
	static u32 GetThreadCount();

private:
	static void GetFeatures();

private:
	static u32 mFeatures;

};

}

