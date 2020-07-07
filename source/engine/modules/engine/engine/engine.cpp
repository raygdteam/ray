#include "pch.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>

#include <input/input.hpp>

#include <json.hpp> /* delete this after ray editor */
#include <fstream> /* delete this after ray editor */
#include <sstream> /* delete this after ray editor */

using namespace ray::core;

namespace ray
{

RayEngine::RayEngine() : _engineLoop(nullptr)
{

}

struct mesh
{
	const char* name;

	std::vector<float> position;
	std::vector<float> rotation;
	std::vector<float> scale;

	struct vertex
	{
		float x, y, z;
	};

	std::vector<vertex> vertices;

	struct normal
	{
		float x, y, z;
	};

	std::vector<normal> normals;

	std::vector<int> indices;
};

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	_engineLoop = engineLoop;

	ray::core::IPlatformWindow* window = core::IPlatformWindow::CreateInstance();
	window->Initialize();
	window->CreateWindow("RAY_ENGINE");

	_renderer = new IRenderer;
	_renderer->Initialize(window);
	
	_logger = new Logger("engine");

	input::add_listener(this);

	window->SetWindowVisibility(true);

	_window = window;

	/* delete this after ray editor */
	
	/* using json = nlohmann::json;

	std::ifstream file("scene.ray_level.meshes");
	json config = json::parse(file);

	std::vector<mesh> meshes;

	for (auto object : config.items())
	{
		mesh seva;

		seva.name = object.key().c_str();

		for (auto coordinate : config[object.key()]["position"])
			seva.position.push_back(coordinate);

		for (auto coordinate : config[object.key()]["rotation"])
			seva.rotation.push_back(coordinate);

		for (auto coordinate : config[object.key()]["scale"])
			seva.scale.push_back(coordinate);

		for (auto vertex : config[object.key()]["vertices"])
		{
			mesh::vertex mesh_vertex;

			mesh_vertex.x = vertex[0];
			mesh_vertex.y = vertex[1];
			mesh_vertex.z = vertex[2];

			seva.vertices.push_back(mesh_vertex);
		}

		for (auto normal : config[object.key()]["normals"])
		{
			mesh::normal mesh_normal;

			mesh_normal.x = normal[0];
			mesh_normal.y = normal[1];
			mesh_normal.z = normal[2];

			seva.normals.push_back(mesh_normal);
		}

		for (auto index : config[object.key()]["indices"])
			seva.indices.push_back(index);

		meshes.push_back(seva);
	} */

	/* delete this after ray editor */
}

void RayEngine::Tick()
{
	input::update();

	static_cast<core::IPlatformWindow*>(_window)->Update();
	//for debugging
	bool bShouldClose = static_cast<core::IPlatformWindow*>(_window)->ShouldClose();
	if (bShouldClose)
	{
		ray::RequestEngineExit(true);
		return;
	}
	 _renderer->Draw();
}

void RayEngine::on_key_down(int key)
{
}

void RayEngine::on_key_up(int key)
{
}

void RayEngine::on_mouse_move(int x, int y)
{

}

void RayEngine::on_left_mouse_down(int x, int y)
{
}

void RayEngine::on_left_mouse_up(int x, int y)
{
}

void RayEngine::on_right_mouse_down(int x, int y)
{
}

void RayEngine::on_right_mouse_up(int x, int y)
{
}

RayEngine::~RayEngine()
{
	static_cast<core::IPlatformWindow*>(_window)->Destroy();
	static_cast<core::IPlatformWindow*>(_window)->Shutdown();
	_renderer->Shutdown();

	delete _renderer;
}

/************************************/
bool gEngineExitRequested = false;

bool IsEngineExitRequested() noexcept
{
	return gEngineExitRequested;
}

void RequestEngineExit(bool data)
{
	gEngineExitRequested = data;
}
}


// void _DllMainCRTStartup() {}
