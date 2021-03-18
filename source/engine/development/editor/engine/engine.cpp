#include <editor/engine/engine.hpp>
#include <core/log/log.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <engine/ui2/ext/imgui.h>
#include <engine/ui2/ext/imgui_internal.h>

#include <chrono>
#include <engine/world/actors/static_quad_actor.hpp>
#include <engine/world/world.hpp>
#include <editor_core/caches/component_cache.hpp>
#include <editor_core/caches/actor_cache.hpp>
#include <renderer_core/resources/buffer_manager.hpp>
#include <renderer_core/command_context.hpp>


#include "editor/windows/debug_window.hpp"
#include <editor/windows/level_viewport.hpp>

#include "editor/windows/level_outline.hpp"
#include "editor/windows/log_window.hpp"
#include "engine/ui2/ui2.hpp"
#include "renderer_core/renderer_2d.hpp"

#undef CreateWindow

IRenderer* gRenderer;
UiRootObject* gRootObject;

EditorEngine* gEditorEngine;

void EditorEngine::Initialize(IEngineLoop* engineLoop)
{
	gEditorEngine = this;
	_window = IPlatformWindow::CreateInstance();

	_window->Initialize();
	_window->CreateWindow("RAY_EDITOR");

	gComponentCache = new ComponentCache;
	gComponentCache->Rebuild();

	gActorCache = new ActorCache();
	gActorCache->Rebuild();
	
	gRenderer = new IRenderer();
	gRenderer->Initialize(_window);

	_world = new World();
	_world->Initialize(nullptr);
	_level = _world->_levelData->Level;
	gRootObject = new UiRootObject();

	_world->RendererInitialize(nullptr);
	
	gRootObject->Initialize(_window);
	
	//_renderer = new IVkRenderer();
	//_renderer->Initialize(_window);

	gRootObject->AddWindow(new EdDebugWindow());
	gRootObject->AddWindow(new EdLevelViewport());
	gRootObject->AddWindow(new EdLevelOutline());
	gRootObject->AddWindow(new EdLogWindow());

	_window->SetWindowVisibility(true);
}

void EditorEngine::Tick()
{
	auto __start = std::chrono::high_resolution_clock::now();

	_window->Update();
	if (_window->ShouldClose())
	{
		RequestEngineExit(true);
		return;
	}

	gRootObject->Tick();

	//_level->GetActors()[0]->GetTransform()->Position.x -= 1.f * _delta;
	//_renderer->BeginScene();
	
	//ImGui::ShowDemoWindow();
	
	/*ImGuiViewport* viewport = ImGui::GetMainViewport();
	if (ImGui::DockBuilderGetNode(1) == nullptr)
	{
		ImGui::DockBuilderAddNode(1, ImGuiDockNodeFlags_DockSpace);
		u32 mainId = 1;

		ImGui::DockBuilderDockWindow("TestLevel", mainId);
		ImGui::DockBuilderFinish(1);
	}
	
	if (ImGui::DockBuilderGetNode(2) == nullptr)
	{
		ImGui::DockBuilderAddNode(2, ImGuiDockNodeFlags_DockSpace);
		u32 mainId = 2;
		u32 bottom = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Down, 0.20f, nullptr, &mainId);
		u32 right = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Right, 0.20f, nullptr, &mainId);

		ImGui::DockBuilderDockWindow("Level Outline", right);
		ImGui::DockBuilderDockWindow("Log", bottom);
		ImGui::DockBuilderFinish(2);
	}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DOCKSPACE", nullptr, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGui::DockSpace(1, ImVec2(0, 0));
	if (ImGui::BeginMenuBar())
	{
		ImGui::Text("FPS: %i", u32(1000.f / delta));
		ImGui::Separator();

		ImGui::EndMenuBar();
	}
	ImGui::End();

	ImGui::Begin("TestLevel");
	ImGui::DockSpace(2, ImVec2(0, 0));
	ImGui::End();

	ImGui::Begin("Log");
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::TextUnformatted(GetLogsAll().AsRawStr());
	ImGui::EndChild();
	ImGui::End();
	
	ImGui::Begin("Level Outline");

	if (ImGui::Button("Add actor"))
	{
		_level->SpawnActor(new StaticQuadActor());
	}

	Array<Actor*>& actors = _level->GetActors();
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
	
	ImGui::End();*/
	
	//_renderer->EndScene();
	
	GraphicsContext& ctx = GraphicsContext::Begin();

	gRenderer->Begin(gSceneColorBuffer, ctx);
	{
		_world->RenderEditor(ctx);
	}
	gRenderer->End(gSceneColorBuffer, ctx);
	
	gRenderer->Begin(gEditorColorBuffer, ctx);
	gRootObject->RenderAll(ctx);
	gRenderer->End(gEditorColorBuffer, ctx);
	gRenderer->Present(gEditorColorBuffer, ctx);
	
	auto elapsed = std::chrono::high_resolution_clock::now() - __start;
	_delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.f;
}

void EditorEngine::ApplyMouseDragOnViewport(FVector2 drag)
{	
	_world->_primaryCameraActor->GetTransform()->Position.x -= drag.x * MouseDragSensitivity;
	_world->_primaryCameraActor->GetTransform()->Position.y += drag.y * MouseDragSensitivity;

	_world->_primaryCameraActor->GetCameraComponent()->UpdateMVP();
}

FVector2& EditorEngine::GetCameraPos()
{
	return _world->_primaryCameraActor->GetTransform()->Position;
}
