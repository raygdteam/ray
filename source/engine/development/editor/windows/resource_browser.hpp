#pragma once
#include <engine/ui2/ui2.hpp>
#include <resources/resource.hpp>

struct Directory;

class EdResourceBrowser : public UiWindow
{
	FVector2 _itemSize = { 75.f, 75.f };
	
	void AppendToFilter(String& dirName);
	void DrawDirectory(Directory* dir, String& directoryName);
	void DrawResource(IRResource* res, String& name);

public:
	EdResourceBrowser();
	void Tick() override;
};
