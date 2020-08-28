#pragma once
#include <d3d12.h>
#include <core/core.hpp>
#include <core/lib/array.hpp>
#include <core/debug/assert.hpp>

namespace ray::renderer_core_api
{
	class RootSignatureParameter
	{
		D3D12_ROOT_PARAMETER _parameter;
	public:
		RootSignatureParameter();
		~RootSignatureParameter() { Clear(); }

		void Clear()
		{
			if (_parameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
				delete[] _parameter.DescriptorTable.pDescriptorRanges;
		
			_parameter.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(UINT_MAX);
		}

		/**
		 * Initialize as array of constant 32-bit values.
		 */
		void InitAsConstants(u32 _register, u32 num32BitValues, D3D12_SHADER_VISIBILITY visibility)
		{
			_parameter = D3D12_ROOT_PARAMETER {
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
				.ShaderVisibility = visibility,
			};

			_parameter.Constants = D3D12_ROOT_CONSTANTS {
				.ShaderRegister = _register,
				.RegisterSpace = 0, // ?
				.Num32BitValues = num32BitValues
			};
		}

		/**
		 * Initialize as buffer.
		 */
		void InitAsConstantBuffer(u32 _register, D3D12_SHADER_VISIBILITY visibility)
		{
			_parameter = D3D12_ROOT_PARAMETER {
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
				.ShaderVisibility = visibility,
			};
			
			_parameter.Descriptor = D3D12_ROOT_DESCRIPTOR {
				.ShaderRegister = _register,
				.RegisterSpace = 0
			};
		}

		/**
		 * Initialize as shader resource view.
		 */
		void InitAsSRVBuffer(u32 _register, D3D12_SHADER_VISIBILITY visibility)
		{
			_parameter = D3D12_ROOT_PARAMETER {
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV,
				.ShaderVisibility = visibility,
			};

			_parameter.Descriptor = D3D12_ROOT_DESCRIPTOR {
				.ShaderRegister = _register,
				.RegisterSpace = 0
			};
		}

		/**
		 * Initialize as unordered access view.
		 */
		void InitAsUAVBuffer(u32 _register, D3D12_SHADER_VISIBILITY visibility)
		{
			_parameter = D3D12_ROOT_PARAMETER {
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV,
				.ShaderVisibility = visibility,
			};

			_parameter.Descriptor = D3D12_ROOT_DESCRIPTOR {
				.ShaderRegister = _register,
				.RegisterSpace = 0
			};
		}

		/**
		 * Initialize as reference to descriptor table.
		 */
		void InitAsDescriptorRange(u32 _register, D3D12_DESCRIPTOR_RANGE_TYPE type, u32 count, D3D12_SHADER_VISIBILITY visibility)
		{
			InitAsDescriptorTable(1, visibility);
			SetTableRange(0, type, _register, count);
		}

		void SetTableRange(u32 rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE type, u32 _register, u32 count, u32 space = 0)
		{
			D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(_parameter.DescriptorTable.pDescriptorRanges + rangeIndex);
			range->RangeType = type;
			range->NumDescriptors = count;
			range->BaseShaderRegister = _register;
			range->RegisterSpace = space;
			range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		}
		
		void InitAsDescriptorTable(u32 rangeCount, D3D12_SHADER_VISIBILITY visibility)
		{
			_parameter = D3D12_ROOT_PARAMETER {
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
				.ShaderVisibility = visibility,
			};
			
			_parameter.DescriptorTable = D3D12_ROOT_DESCRIPTOR_TABLE {
				.NumDescriptorRanges = rangeCount,
				.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[rangeCount],
			};
		}

	};
	
	class RootSignature
	{
	public:
		void Begin(u32 numParams, u32 numStaticSamplers);
		void Finalize(D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

		void InitStaticSampler(u32 shaderRegister, const D3D12_SAMPLER_DESC& samplerDesc, D3D12_SHADER_VISIBILITY visibility);
		
		ID3D12RootSignature* GetRootSignature() const noexcept { return _rootSignature; }

		RootSignatureParameter& Slot(size_t index)
		{
			check(index < _rootParameters.Size());
			return _rootParameters.At(index);
		}

	private:
		ID3D12RootSignature* _rootSignature;
		Array<RootSignatureParameter> _rootParameters;
		Array<D3D12_STATIC_SAMPLER_DESC> _staticSampler;
		size_t _numInitializedStaticSamplers;

	};

}
