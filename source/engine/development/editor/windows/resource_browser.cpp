#include "resource_browser.hpp"
#include "editor/engine/engine.hpp"
#include "engine/state/state.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/ui2/ext/imgui_internal.h"
#include "resources/resource_manager.hpp"

String gFilter("/");
const String gSlash("/");

void SplitPath(String path, Array<String>& out)
{
	path = String(path.substr(1));
	if (path.find_first_of("/") != String::npos)
	{
		while (path.find_first_of("/") != String::npos)
		{
			size_t n = path.find_first_of("/");
			out.PushBack(String(path.substr(0, n)));
			path = String(path.substr(n + 1));
		}

		// there is only one left
		out.PushBack(path);
	}
	else
	{
		// rpath contains the only component
		if (!path.empty())
		{
			out.PushBack(path);
		}
	}
}

void EdResourceBrowser::AppendToFilter(String& dirName)
{
	if (dirName == String(".."))
	{
		Array<String> pathComponents;
		SplitPath(gFilter, pathComponents);
		
		String newFilter("/");

		for (size_t i = 0; i < pathComponents.Size() - 1; ++i)
		{
			newFilter.append_sprintf("%s/", pathComponents[i].AsRawStr());
		}

		if (newFilter != gSlash && newFilter.length() != 0 && newFilter[strlen(newFilter.data()) - 1] == '/')
			newFilter = String(newFilter.substr(0, newFilter.Length() - 1));

		gFilter = newFilter;
		return;
	}
	
	if (gFilter == gSlash || gFilter[strlen(gFilter.AsRawStr()) - 1] == '/')
	{
		gFilter.append_sprintf("%s", dirName.AsRawStr());
	}
	else
	{
		gFilter.append_sprintf("/%s", dirName.AsRawStr());
	}
}

void EdResourceBrowser::DrawDirectory(Directory* dir, String& directoryName)
{
	ImGuiContext& g = *GImGui;
	float labelHeight = g.FontSize;

	ImGui::BeginGroup();
	ImGui::PushID(dir);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.25f));

	ImRect rectButton = ImRect
	(
		ImGui::GetCursorScreenPos().x,
		ImGui::GetCursorScreenPos().y,
		ImGui::GetCursorScreenPos().x + _itemSize.x,
		ImGui::GetCursorScreenPos().y + _itemSize.y
	);

	ImRect rectLabel = ImRect
	(
		rectButton.Min.x,
		rectButton.Max.y - labelHeight - ImGui::GetStyle().FramePadding.y,
		rectButton.Max.x,
		rectButton.Max.y
	);

	if (ImGui::Button("##dummy", ImVec2(_itemSize.x, _itemSize.y)))
	{
		
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		AppendToFilter(directoryName);
	}

	ImVec2 maxImageSize = ImVec2(rectButton.Max.x - rectButton.Min.x - ImGui::GetStyle().FramePadding.x * 2.0f, rectButton.Max.y - rectButton.Min.y - ImGui::GetStyle().FramePadding.y - labelHeight - 5.0f);
	ImVec2 imageSize = maxImageSize;
	ImVec2 deltaImageSize;

	if (imageSize.x != maxImageSize.x)
	{
		float scale = maxImageSize.x / imageSize.x;
		imageSize.x = maxImageSize.x;
		imageSize.y = imageSize.y * scale;
	}
	// Clamp height
	if (imageSize.y != maxImageSize.y)
	{
		float scale = maxImageSize.y / imageSize.y;
		imageSize.x = imageSize.x * scale;
		imageSize.y = maxImageSize.y;
	}

	deltaImageSize.x = maxImageSize.x - imageSize.x;
	deltaImageSize.y = maxImageSize.y - imageSize.y;

	ImGui::SetCursorScreenPos(ImVec2(rectButton.Min.x + ImGui::GetStyle().FramePadding.x + deltaImageSize.x * 0.5f, rectButton.Min.y + ImGui::GetStyle().FramePadding.y + deltaImageSize.y * 0.5f));
	ImGui::Image(0, imageSize);

	ImGui::PopStyleColor(2);
	ImGui::PopID();
	
	const ImVec2 label_size = ImGui::CalcTextSize(directoryName.AsRawStr(), nullptr, true);

	ImGui::GetWindowDrawList()->AddRectFilled(rectLabel.Min, rectLabel.Max, IM_COL32(51, 51, 51, 190));
	//ImGui::GetWindowDrawList()->AddRect(rect_label.Min, rect_label.Max, IM_COL32(255, 0, 0, 255)); // debug

	// Draw text
	ImGui::SetCursorScreenPos(ImVec2(rectLabel.Min.x + 3.0f, rectLabel.Min.y + 3.0f));
	if (label_size.x <= _itemSize.x && label_size.y <= _itemSize.y)
	{
		ImGui::TextUnformatted(directoryName.AsRawStr());
	}
	else
	{
		ImGui::RenderTextClipped(rectLabel.Min, rectLabel.Max, directoryName.AsRawStr(), nullptr, &label_size, ImVec2(0, 0), &rectLabel);
	}
	ImGui::EndGroup();
}

void EdResourceBrowser::DrawResource(IRResource* res, String& name)
{
	ImGuiContext& g = *GImGui;
	float labelHeight = g.FontSize;

	ImGui::BeginGroup();
	ImGui::PushID(res);
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.25f));

	ImRect rectButton = ImRect
	(
		ImGui::GetCursorScreenPos().x,
		ImGui::GetCursorScreenPos().y,
		ImGui::GetCursorScreenPos().x + _itemSize.x,
		ImGui::GetCursorScreenPos().y + _itemSize.y
	);

	ImRect rectLabel = ImRect
	(
		rectButton.Min.x,
		rectButton.Max.y - labelHeight - ImGui::GetStyle().FramePadding.y,
		rectButton.Max.x,
		rectButton.Max.y
	);

	if (ImGui::Button("##dummy", ImVec2(_itemSize.x, _itemSize.y)))
	{

	}

	if (ImGui::BeginDragDropSource())
	{
		ImGui::Text("Dragging %s...", name.AsRawStr());
		String type(res->GetType()->Name);
		if (type.Length() >= 32) type = String(type.substr(0, 32));
		ImGui::SetDragDropPayload(type.AsRawStr(), &res, sizeof(void*));
		ImGui::EndDragDropSource();
	}

	ImVec2 maxImageSize = ImVec2(rectButton.Max.x - rectButton.Min.x - ImGui::GetStyle().FramePadding.x * 2.0f, rectButton.Max.y - rectButton.Min.y - ImGui::GetStyle().FramePadding.y - labelHeight - 5.0f);
	ImVec2 imageSize = maxImageSize;
	ImVec2 deltaImageSize;

	if (imageSize.x != maxImageSize.x)
	{
		float scale = maxImageSize.x / imageSize.x;
		imageSize.x = maxImageSize.x;
		imageSize.y = imageSize.y * scale;
	}

	if (imageSize.y != maxImageSize.y)
	{
		float scale = maxImageSize.y / imageSize.y;
		imageSize.x = imageSize.x * scale;
		imageSize.y = maxImageSize.y;
	}

	deltaImageSize.x = maxImageSize.x - imageSize.x;
	deltaImageSize.y = maxImageSize.y - imageSize.y;

	ImGui::SetCursorScreenPos(ImVec2(rectButton.Min.x + ImGui::GetStyle().FramePadding.x + deltaImageSize.x * 0.5f, rectButton.Min.y + ImGui::GetStyle().FramePadding.y + deltaImageSize.y * 0.5f));
	ImGui::Image((ImTextureID)1, imageSize);

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	const ImVec2 label_size = ImGui::CalcTextSize(name.AsRawStr(), nullptr, true);

	ImGui::GetWindowDrawList()->AddRectFilled(rectLabel.Min, rectLabel.Max, IM_COL32(51, 51, 51, 190));
	//ImGui::GetWindowDrawList()->AddRect(rect_label.Min, rect_label.Max, IM_COL32(255, 0, 0, 255)); // debug

	ImGui::SetCursorScreenPos(ImVec2(rectLabel.Min.x + 3.0f, rectLabel.Min.y + 3.0f));
	if (label_size.x <= _itemSize.x && label_size.y <= _itemSize.y)
	{
		ImGui::TextUnformatted(name.AsRawStr());
	}
	else
	{
		ImGui::RenderTextClipped(rectLabel.Min, rectLabel.Max, name.AsRawStr(), nullptr, &label_size, ImVec2(0, 0), &rectLabel);
	}
	
	ImGui::EndGroup();
}

EdResourceBrowser::EdResourceBrowser()
{
	Title = String("Resource Browser");
}

void EdResourceBrowser::Tick()
{
	//ImGui::TextUnformatted(gFilter.AsRawStr());
	//ImGui::Separator();

	Directory& rootDir = RayState()->ResourceManager->GetRootDirectory();

	String filter(gFilter);
	if (filter != gSlash && filter.length() != 0 && filter[strlen(filter.data()) - 1] == '/')
		filter = String(filter.substr(0, filter.Length() - 1));

	Array<String> pathComponents;
	SplitPath(filter, pathComponents);

	Directory* dir = &rootDir;
	
	if (!pathComponents.empty())
	{
		bool bFirst = true;

		for (String& pathComponent : pathComponents)
		{
			if (bFirst)
			{
				dir = &rootDir.Directories[pathComponent];
				bFirst = false;
			}
			else
			{
				dir = &dir->Directories[pathComponent];
			}
		}
	}
	else
	{}


	const float contentWidth = ImGui::GetContentRegionAvail().x;
	const float contentHeight = ImGui::GetContentRegionAvail().y - 10.f;

	if (ImGui::BeginChild("##Content", ImVec2(contentWidth, contentHeight)))
	{
		float penX = 0.0f;
		float penXMin = ImGui::GetCursorPosX() + ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorPosX(penXMin);
		bool newLine = true;

		if (filter != gSlash)
		{
			if (newLine)
			{
				ImGui::BeginGroup();
				newLine = false;
			}
			
			String name("..");
			void* dummy = &gFilter;
			DrawDirectory((Directory*)dummy, name);
			
			penX += _itemSize.x + ImGui::GetStyle().ItemSpacing.x;
			if (penX >= contentWidth - _itemSize.x)
			{
				ImGui::EndGroup();
				penX = penXMin;
				ImGui::SetCursorPosX(penX);
				newLine = true;
			}
			else
			{
				ImGui::SameLine();
			}
		}

		for (eastl::pair<const String, Directory>& pair : dir->Directories)
		{
			if (newLine)
			{
				ImGui::BeginGroup();
				newLine = false;
			}

			String name(pair.first);
			DrawDirectory(&pair.second, name);

			penX += _itemSize.x + ImGui::GetStyle().ItemSpacing.x;
			if (penX >= contentWidth - _itemSize.x)
			{
				ImGui::EndGroup();
				penX = penXMin;
				ImGui::SetCursorPosX(penX);
				newLine = true;
			}
			else
			{
				ImGui::SameLine();
			}
		}

		for (eastl::pair<const String, ResourceData>& pair : dir->Resources)
		{
			if (newLine)
			{
				ImGui::BeginGroup();
				newLine = false;
			}

			String name(pair.first);
			DrawResource(pair.second.ResourceRef, name);

			penX += _itemSize.x + ImGui::GetStyle().ItemSpacing.x;
			if (penX >= contentWidth - _itemSize.x)
			{
				ImGui::EndGroup();
				penX = penXMin;
				ImGui::SetCursorPosX(penX);
				newLine = true;
			}
			else
			{
				ImGui::SameLine();
			}

		}

		if (!newLine)
			ImGui::EndGroup();
	}
	
	ImGui::EndChild();
}


