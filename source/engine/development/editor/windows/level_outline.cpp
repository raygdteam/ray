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
					Transform* obj = (Transform*)component;
					FVector2* pos = &obj->Position;

					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted("x");
					ImGui::NextColumn();

					ImGui::PushID(&pos->x);
					ImGui::SliderFloat("##value", &pos->x, -1000.f, 1000.f);
					ImGui::PopID();

					ImGui::NextColumn();

					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted("y");
					ImGui::NextColumn();

					ImGui::PushID(&pos->y);
					ImGui::SliderFloat("##value", &pos->y, -1000.f, 1000.f);
					ImGui::PopID();

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

