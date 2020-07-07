#pragma once
#include <core/core.hpp>

namespace ray::renderer_core_api
{
	class IShader
	{
	public:
		virtual bool Compile(pcstr) = 0;
		virtual bool LoadBytecode(pcstr) = 0;
		
		void* GetBuffer() { return _shader_buffer; }
		void* GetBytecode() { return _shader_bytecode; }

		void SetBuffer(void* buffer) { _shader_buffer = buffer; }
		void SetBytecode(void* bytecode) { _shader_bytecode = bytecode; }

	protected:
		void* _shader_buffer;
		void* _shader_bytecode;
	};
}