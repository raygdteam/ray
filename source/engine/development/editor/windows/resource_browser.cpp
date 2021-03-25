#include "resource_browser.hpp"
#include "editor/engine/engine.hpp"
#include "engine/state/state.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/ui2/ext/imgui_internal.h"
#include "resources/resource_manager.hpp"

String gFilter("/engine/");

EdResourceBrowser::EdResourceBrowser()
{
	Title = String("Resource Browser");
}

void EdResourceBrowser::Tick()
{
	InputText("gFilter", &gFilter, 0, nullptr, nullptr);
	ImGui::Separator();

	Array<ResourceData>& resources = RayState()->ResourceManager->GetAllResources();

	String filter(gFilter);
	if (filter.length() != 0 && filter[strlen(filter.data()) - 1] == '/')
		filter = String(filter.substr(0, filter.Length() - 1));
	
	for (ResourceData& resource : resources)
	{
		String resourcePath(resource.Name.substr(0, resource.Name.find_last_of("/")));

		if (filter == resourcePath)
		{
			ImGui::TextUnformatted(resource.Name.AsRawStr());
		}
	}
	
}


