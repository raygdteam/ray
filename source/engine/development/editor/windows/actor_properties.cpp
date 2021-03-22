#include "actor_properties.hpp"
#include "level_outline.hpp"
#include "editor_core/caches/component_cache.hpp"
#include "engine/ui2/ext/imgui.h"

// TODO: move it to common
struct InputTextCallback_UserData
{
    String* Str;
    ImGuiInputTextCallback ChainCallback;
    void* ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        String* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool InputText(const char* label, String* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

static Type* gCurrentlyChosenComponent = nullptr;

EdActorProperties::EdActorProperties()
{
	Title = String("Actor Properties");
}

void EdActorProperties::Tick()
{
	if (gSelectedActor == nullptr)
	{
		ImGui::Text("No Actor selected.");
		return;
	}

	Actor* actor = gSelectedActor;

	ImGui::TextUnformatted("Name "); ImGui::SameLine();
	InputText("##ActorNameEnter", &actor->GetName(), 0, 0, 0);

	ImGui::TextUnformatted("Active"); ImGui::SameLine();
	ImGui::Checkbox("##ActorCbActive", &actor->ATD.Active);

	ImGui::Separator();

	for (IComponent* component : actor->GetComponents())
	{
		ImGui::Columns(1);
		ImGui::AlignTextToFramePadding();
		bool openProps = ImGui::TreeNodeEx(component + 1, ImGuiTreeNodeFlags_None, "%s", component->GetType()->Name);
		if (openProps)
		{
			Type* type = component->GetType();
			TypeHelper helper(type, component);

			if (type->Fields.IsEmpty())
			{
				ImGui::TextUnformatted("There are no availiable properties for this Component.");
				ImGui::TreePop();
			}
			else
			{
				//ImGui::PushID(component + 2);
				ImGui::Columns(2);
				for (FieldInfo& field : type->Fields)
				{
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(field.Name.AsRawStr());
					ImGui::NextColumn();

					switch (field.Type)
					{
					case eU8:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_U8, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eU16:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_U16, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eU32:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_U32, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eU64:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_U64, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eS8:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_S8, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eS16:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_S16, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eS32:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_S32, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eS64:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_S64, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eF32:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_Float, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eF64:
						ImGui::DragScalar(field.Name.AsRawStr(), ImGuiDataType_Double, (void*)((u8*)component + field.Offset), 1.f);
						break;
					case eBoolean:
						ImGui::Checkbox(field.Name.AsRawStr(), (bool*)((u8*)component + field.Offset));
						break;
					case eVector1:
						ImGui::DragScalar("x", ImGuiDataType_Float, &((FVector1*)((u8*)component + field.Offset))->x, 1.f);
						break;
					case eVector2:
						ImGui::PushID(&((FVector2*)((u8*)component + field.Offset))->x);
						ImGui::DragScalar("x", ImGuiDataType_Float, &((FVector2*)((u8*)component + field.Offset))->x, 1.f);
						ImGui::PopID();
						//ImGui::SameLine();
						ImGui::PushID(&((FVector2*)((u8*)component + field.Offset))->y);
						ImGui::DragScalar("y", ImGuiDataType_Float, &((FVector2*)((u8*)component + field.Offset))->y, 1.f);
						ImGui::PopID();
						break;
					case eVector3:
						ImGui::PushID(&((FVector3*)((u8*)component + field.Offset))->x);
						ImGui::DragScalar("x", ImGuiDataType_Float, &((FVector3*)((u8*)component + field.Offset))->x, 1.f);
						ImGui::PopID();
						//ImGui::SameLine();
						ImGui::PushID(&((FVector3*)((u8*)component + field.Offset))->y);
						ImGui::DragScalar("y", ImGuiDataType_Float, &((FVector3*)((u8*)component + field.Offset))->y, 1.f);
						ImGui::PopID();
						//ImGui::SameLine();
						ImGui::PushID(&((FVector3*)((u8*)component + field.Offset))->z);
						ImGui::DragScalar("z", ImGuiDataType_Float, &((FVector3*)((u8*)component + field.Offset))->z, 1.f);
						ImGui::PopID();
						break;
					case eVector4:
						ImGui::DragScalar("x", ImGuiDataType_Float, &((FVector4*)((u8*)component + field.Offset))->x, 1.f);
						//ImGui::SameLine();
						ImGui::DragScalar("y", ImGuiDataType_Float, &((FVector4*)((u8*)component + field.Offset))->y, 1.f);
						//ImGui::SameLine();
						ImGui::DragScalar("z", ImGuiDataType_Float, &((FVector4*)((u8*)component + field.Offset))->z, 1.f);
						//ImGui::SameLine();
						ImGui::DragScalar("w", ImGuiDataType_Float, &((FVector4*)((u8*)component + field.Offset))->w, 1.f);
						break;
					default:
						break;
					}
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
				//ImGui::PopID();
				ImGui::TreePop();
			}
		}
	}

	ImGui::Separator();
	if (ImGui::Button("Add Component..."))
	{
		ImGui::OpenPopup("Add Component");
	}

	if (ImGui::BeginPopupModal("Add Component"))
	{
		Array<Type*>& cache = gComponentCache->GetCache();

		for (Type* type : cache)
		{
			if (ImGui::Selectable(type->Name, gCurrentlyChosenComponent == type, ImGuiSelectableFlags_DontClosePopups))
			{
				gCurrentlyChosenComponent = type;
			}
		}

		if (ImGui::Button("OK"))
		{
			gSelectedActor->GetComponents().PushBack((IComponent*)gCurrentlyChosenComponent->CreateInstance());
			gCurrentlyChosenComponent = nullptr;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			gCurrentlyChosenComponent = nullptr;
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
}