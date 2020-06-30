#pragma once
#include <core/core.hpp>

#include <renderer_core/command_allocator.hpp>
#include <renderer_core/command_list.hpp>
#include <renderer_core/command_queue.hpp>
#include <renderer_core/descriptor_heap.hpp>
#include <renderer_core/device.hpp>
#include <renderer_core/fence.hpp>
#include <renderer_core/pipeline_state.hpp>
#include <renderer_core/swap_chain.hpp>
#include <renderer_core/resources/resources.hpp>

using namespace ray::renderer_core_api;
using namespace ray::renderer_core_api::resources;

#include <d3d12.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <combaseapi.h>
#include <dxgi1_4.h>

#include "d3dx12.h"

#define CastInstanceTo(Type, owner) static_cast<Type*>(owner->GetInstance())