#include "pch.hpp"
#include "platform/platform_processor.hpp"
#include "memory/memory.hpp"
#include <ray/os/include.hpp>

namespace ray::core::platform
{

struct registers
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
};

std::string PlatformProcessor::GetModelName()
{
	// @see for more information http://msdn.microsoft.com/en-us/library/vstudio/hskdteyh(v=vs.100).aspx
	static char model_name[0x40];
	registers regs;
	const size_t regs_size = sizeof(regs);

	__cpuid((s32*)&regs, 0x80000000);
	const u32 max_ext_ids = regs.eax;

	if (max_ext_ids >= 0x80000004)
	{
		const u32 first_model_string = 0x80000002;
		const u32 NumBrandStrings = 3;
		for (u32 index = 0; index < NumBrandStrings; index++)
		{
			__cpuid((s32*)&regs, first_model_string + index);
			memory::Memory::Memcpy(model_name + regs_size * index, (s32*)&regs, regs_size);
		}
	}

	return std::string(model_name);
}

u16 PlatformProcessor::GetCacheLineSize()
{
	s32 CacheLineSize = 1;

	s32 Args[4];
	__cpuid(Args, 0x80000006);

	CacheLineSize = Args[2] & 0xFF;

	return CacheLineSize;
}

u32 PlatformProcessor::GetCoreCount()
{
	
}

u32 PlatformProcessor::GetThreadCount()
{
	
}

bool PlatformProcessor::HasFeature(Feature f)
{
	static bool sFirst = true;
	if (sFirst)
	{
		GetFeatures();
		sFirst = false;
	}

	return mFeatures & static_cast<u32>(f);
}

void PlatformProcessor::GetFeatures()
{
	registers regs;
	__cpuid((s32*)&regs, 1);

	if (regs.edx & (1 << 25) || false)
		mFeatures |= static_cast<u32>(Feature::SSE);

	if (regs.edx & (1 << 26) || false)
		mFeatures |= static_cast<u32>(Feature::SSE2);

	if (regs.ecx & (1 << 0) || false)
		mFeatures |= static_cast<u32>(Feature::SSE3);

	if (regs.ecx & (1 << 9) || false)
		mFeatures |= static_cast<u32>(Feature::SSSE3);

	if (regs.ecx & (1 << 19) || false)
		mFeatures |= static_cast<u32>(Feature::SSE4_1);

	if (regs.ecx & (1 << 20) || false)
		mFeatures |= static_cast<u32>(Feature::SSE4_2);

	if (regs.ecx & (1 << 28) || false)
		mFeatures |= static_cast<u32>(Feature::AVX);

	//memset(&regs, 0, sizeof(registers));

	__cpuid((s32*)&regs, 0x80000000);
	s32 numExtendedIds = regs.eax;

	if (numExtendedIds >= 0x80000001)
	{
		__cpuid((s32*)&regs, 0x80000001);

		if (regs.ecx & (1 << 6) || false)
			mFeatures |= static_cast<u32>(Feature::SSE4A);

		if (regs.ecx & (1 << 11) || false)
			mFeatures |= static_cast<u32>(Feature::SSE5);
	}
}

}
