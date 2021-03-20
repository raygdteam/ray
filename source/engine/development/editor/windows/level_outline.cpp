#include "level_outline.hpp"


#include "editor/engine/engine.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/world/actors/static_quad_actor.hpp"

EdLevelOutline::EdLevelOutline()
{
	Title = String("Level Outline");
}

void EdLevelOutline::Tick()
{
	Level* level = gEditorEngine->GetLevel();

	ImGui::Begin("Level Outline");

	if (ImGui::Button("Add actor"))
	{
		level->SpawnActor(new StaticQuadActor());
	}

	Array<Actor*>& actors = level->GetActors();
	ImGui::Columns(2);
	ImGui::Separator();

	for (Actor* actor : actors)
	{
		ImGui::PushID(actor);
		ImGui::Columns(2);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode("Actor", "%s", actor->GetName().AsRawStr());
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(actor->GetType()->Name);
		ImGui::NextColumn();

		if (node_open)
		{
			for (IComponent* component : actor->GetComponents())
			{
				ImGui::Columns(1);
				ImGui::PushID(component);
				ImGui::AlignTextToFramePadding();
				bool openProps = ImGui::TreeNodeEx(component + 1, ImGuiTreeNodeFlags_None, "%s", component->GetType()->Name);
				if (openProps)
				{
					ImGui::PushID(component + 2);
					ImGui::Columns(2);
					//ImGui::NextColumn();
					//Transform* obj = (Transform*)component;
					//FVector2* pos = &obj->Position;
					Type* type = component->GetType();
					TypeHelper helper(type, component);
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
							//ImGui::SameLine();
							ImGui::PushID(&((FVector3*)((u8*)component + field.Offset))->y);
							ImGui::DragScalar("y", ImGuiDataType_Float, &((FVector3*)((u8*)component + field.Offset))->y, 1.f);
							//ImGui::SameLine();
							ImGui::PushID(&((FVector3*)((u8*)component + field.Offset))->z);
							ImGui::DragScalar("z", ImGuiDataType_Float, &((FVector3*)((u8*)component + field.Offset))->z, 1.f);
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

					//ImGui::AlignTextToFramePadding();
					//ImGui::TextUnformatted("x");
					//ImGui::NextColumn();

					//ImGui::PushID(&pos->x);
					//ImGui::DragFloat("##value", &pos->x, 1.f);
					//ImGui::PopID();
					//
					//ImGui::NextColumn();

					//ImGui::AlignTextToFramePadding();
					//ImGui::TextUnformatted("y");
					//ImGui::NextColumn();

					//ImGui::PushID(&pos->y);
					//ImGui::DragFloat("##value", &pos->y, 1.f);
					//ImGui::PopID();

					ImGui::Columns(1);
					ImGui::PopID();

					ImGui::TreePop();
				}

				ImGui::PopID();
				//				ImGui::Columns(2);
			}

			ImGui::TreePop();
		}

		//ImGui::TreePop();
		ImGui::PopID();
	}

	ImGui::End();
}

