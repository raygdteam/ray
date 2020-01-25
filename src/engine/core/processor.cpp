#include "pch.hpp"
#include "processor.h"
#include <memory>
#include "spdlog/spdlog.hpp"
#ifdef RAY_PLATFORM_WIN
#include <intrin.h>
#elif defined(RAY_PLATFORM_LINUX)
#inlcude <cpuid.h>

void __cpuid(s32* regs, s32 func)
{
	__get_cpu_id(func, (u32)regs[0], (u32)regs[1], (u32)regs[2], (u32)regs[3]);
}
#endif

namespace ray::core::hardware
{

	struct registers
	{
		u32 eax;
		u32 ebx;
		u32 ecx;
		u32 edx;
	};

	char* get_vendor_name()
	{
		union
		{
			char s[16];
			s32  i[4];
		} name;
		registers regs;
		__cpuid((s32*)&regs, 0);
		name.i[0] = regs.ebx;
		name.i[1] = regs.edx;
		name.i[2] = regs.ecx;
		name.i[3] = 0;

		char normalizedName[16];
		memcpy(&normalizedName, name.s, 16 * sizeof(char));

		return normalizedName;
	}

	/*
	__cpuid(cpuinfo, 0x80000000);
	s32 numExtendedIds = cpuinfo[0];
	if (numExtendedIds >= 0x80000001)
	{
		__cpuid(cpuinfo, 0x80000001);
		sse4aSupportted = cpuinfo[2] & (1 << 6) || false;
		sse5Supportted = cpuinfo[2] & (1 << 11) || false;
	}
	}
	*/


	unsigned get_cpu_features()
	{
		u32 features = 0;
		registers regs;
		__cpuid((s32*)&regs, 1);

		if (regs.edx & (1 << 25) || false)
			features |= Feature::SSE;

		if (regs.edx & (1 << 26) || false)
			features |= Feature::SSE2;

		if (regs.ecx & (1 << 0) || false)
			features |= Feature::SSE3;

		if (regs.ecx & (1 << 9) || false)
			features |= Feature::SSSE3;

		if (regs.ecx & (1 << 19) || false)
			features |= Feature::SSE4_1;

		if (regs.ecx &(1 << 20) || false)
			features |= Feature::SSE4_2;

		if (regs.ecx & (1 << 28) || false)
			features |= Feature::AVX;

		//memset(&regs, 0, sizeof(registers));

		__cpuid((s32*)&regs, 0x80000000);
		s32 numExtendedIds = regs.eax;

		if (numExtendedIds >= 0x80000001)
		{
			__cpuid((s32*)&regs, 0x80000001);

			if (regs.ecx & (1 << 6) || false)
				features |= Feature::SSE4A;

			if (regs.ecx & (1 << 11) || false)
				features |= Feature::SSE5;
		}

		return features;
	}

	CPU::CacheInfo get_cache_info() //Любезно взято из Unreal Engine 4
	{
		CPU::CacheInfo cache_info;

		s32 cache_line_size = 1;

		s32 Args[4];
		__cpuid(Args, 0x80000006);

		cache_line_size = Args[2] & 0xFF;

		cache_info.cache_line_size = cache_line_size;

		//TODO...

		return cache_info;
	}

	pcstr get_cpu_model_name() //Любезно взято из Unreal Engine 4
	{
		// @see for more information http://msdn.microsoft.com/en-us/library/vstudio/hskdteyh(v=vs.100).aspx
		char* model_name = (char*)malloc(0x40);
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
				memcpy(model_name + regs_size * index, (s32*)&regs, regs_size);
			}
		}
		
		return model_name;
	}

	void init_processor_windows(processor* proc)
	{
		proc->vendor = (strcmp(get_vendor_name(), "GenuineIntel") == 0) ? Vendor::INTEL :
			(strcmp(get_vendor_name(), "AuthenticAMD") == 0) ? Vendor::AMD : Vendor::NONE;
		proc->cpu.features = get_cpu_features();
		proc->cpu.cache = get_cache_info();
		proc->model = get_cpu_model_name();
		/*std::string model_name = proc->model;*/
		// spdlog::info("core: detected cpu {}", proc->model);
	}

	void init_processor_linux(processor* proc)
	{

	}

	bool has_feature(processor* proc, Feature feature)
	{
		return proc->cpu.features & (u32)feature;
	}

	void clear_processor_info(processor* proc)
	{
		free((void*)proc->model);
	}

	void prefetch_block(const void * InPtr, s32 NumBytes) //Любезно взято из Unreal Engine 4
	{
		pcstr Ptr = (pcstr)InPtr;
		const s32 CacheLineSize = get_cache_info().cache_line_size;
		for (s32 LinesToPrefetch = (NumBytes + CacheLineSize - 1) / CacheLineSize; LinesToPrefetch; --LinesToPrefetch)
		{
			_mm_prefetch(Ptr, _MM_HINT_T0);
			Ptr += CacheLineSize;
		}
	}

	void prefetch(void const* x, s32 offset) //Любезно взято из Unreal Engine 4
	{
		_mm_prefetch((pcstr)(x)+offset, _MM_HINT_T0);
	}

}

