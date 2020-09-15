#include <engine/world/world.hpp>
#include <engine/world/level.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_2d.hpp>

struct WorldLevelData
{
	Level* Level;
	/* for level streaming
	 * FVector3 Position;
	 */
};

void World::Render()
{
	Level* level = _levelData->Level;
	const ActorData* actorData = level->_atd.GetData();
	(void)actorData;

	ray::renderer_core_api::Renderer2D::Begin();

	// for (const ActorData& data : actorData)
	// {
	//		data.SceneProxy.RenderData
	//		...
	// }
}
