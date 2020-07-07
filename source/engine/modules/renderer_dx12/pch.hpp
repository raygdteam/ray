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
#include <renderer_core/resources/vertex_buffer.hpp>
#include <renderer_core/resources/index_buffer.hpp>
#include <renderer_core/resources/textures.hpp>
#include <renderer_core/vertex_shader.hpp>
#include <renderer_core/pixel_shader.hpp>
#include <renderer_core/rect.hpp>
#include <renderer_core/root_signature.hpp>
#include <renderer_core/viewport.hpp>

using namespace ray::renderer_core_api;
using namespace ray::renderer_core_api::resources;

#include <d3d12.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <combaseapi.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#include "d3dx12.h"
#include "utils.hpp"