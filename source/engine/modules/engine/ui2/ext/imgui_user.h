#pragma once
#include <engine/engine/engine_def.hpp>
#include <core/lib/string.hpp>

bool RAY_ENGINE_API InputText(const char* label, String* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data);
