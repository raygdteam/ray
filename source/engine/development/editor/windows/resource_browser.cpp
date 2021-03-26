#include "resource_browser.hpp"
#include "editor/engine/engine.hpp"
#include "engine/state/state.hpp"
#include "engine/ui2/ext/imgui.h"
#include "engine/ui2/ext/imgui_internal.h"
#include "resources/resource_manager.hpp"

String gFilter("/");
const String gSlash("/");


EdResourceBrowser::EdResourceBrowser()
{
	Title = String("Resource Browser");
}

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

void EdResourceBrowser::Tick()
{
	ImGui::TextUnformatted(gFilter.AsRawStr());
	ImGui::Separator();

	Array<ResourceData>& resources = RayState()->ResourceManager->GetAllResources();

	String filter(gFilter);
	if (filter != gSlash && filter.length() != 0 && filter[strlen(filter.data()) - 1] == '/')
		filter = String(filter.substr(0, filter.Length() - 1));

	/*******************************************/

	struct File
	{
		IRResource* Resource;
	};

	struct Directory
	{
		eastl::map<String, File> Files;
		eastl::map<String, Directory> Directories;
	};

	Directory rootDir;
	
	for (ResourceData& resource : resources)
	{
		String resourcePath(resource.Name.substr(0, resource.Name.find_last_of("/")));
		String resourceName(resource.Name.substr(resource.Name.find_last_of("/") + 1));
		Array<String> pathComponents;

		SplitPath(resourcePath, pathComponents);

		{
			Directory* dir = &rootDir.Directories[pathComponents[0]];
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
			
			dir->Files[resourceName] = File { resource.ResourceRef };
		}
	}

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

	if (filter != gSlash)
	{	
		if (ImGui::Button(".."))
		{
			String newFilter("/");
			
			for (size_t i = 0; i < pathComponents.Size() - 1; ++i)
			{
				newFilter.append_sprintf("%s/", pathComponents[i].AsRawStr());
			}

			gFilter = newFilter;
		}
	}
	
	for (eastl::pair<const String, Directory>& pair : dir->Directories)
	{
		if (ImGui::Button(pair.first.AsRawStr()))
		{
			filter.append_sprintf(filter == gSlash ? "%s" : "/%s", pair.first.AsRawStr());
			gFilter = filter;
		}
	}
	
	for (eastl::pair<const String, File>& pair : dir->Files)
	{
		ImGui::Text("%s", pair.first.AsRawStr());
	}
}


