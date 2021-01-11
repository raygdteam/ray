#pragma once
#include <core/core.hpp>

namespace ray::renderer_core_api
{
	class IRRCBase
	{
	public:
		//for debugging
		virtual void SetName(pcstr name) { _debug_name = name; }

		void SetInstance(void* instance) { _instance = instance; }
		void* GetInstance() { return _instance; }

	protected:
		void* _instance = nullptr;
		pcstr _debug_name;

	};
}