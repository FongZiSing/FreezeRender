#include "RenderWorld.hpp"

#include <Asset/World/Loader/WorldLoaderLibrary.hpp>
#include <Input/InputSystem.hpp>



/**
 * @brief Internal data of RenderWorld.
 */
inline namespace RenderWorldData
{
	std::unique_ptr<World> world;
}



RenderWorld::RenderWorld()
{
}

RenderWorld::~RenderWorld()
{
}

void RenderWorld::Load()
{
	world = std::make_unique<World>();
	WorldLoaderLibrary::InitializeDefaultWorld(*world);
}


void RenderWorld::Startup(unsigned int screenWidth, unsigned int screenHeight)
{
	world = std::make_unique<World>();
	WorldLoaderLibrary::InitializeDefaultWorld(*world);


	for (auto& perCamera : world->allCamera)
	{
		auto& renderCamera = render.cameras.Emplace(perCamera, screenWidth, screenHeight);
		logic.cameras.Emplace(perCamera, renderCamera);
	}

	for (auto& perMeshlet : world->allMeshlet)
	{
		auto& renderMeshlet = render.meshlets.Emplace(perMeshlet);
		logic.meshlets.Emplace(perMeshlet, renderMeshlet);
	}
	
	for (auto& perPointLight : world->allPointLight)
	{
		auto& renderPointlight = render.pointlights.Emplace(perPointLight);
		logic.pointlights.Emplace(perPointLight, renderPointlight);
	}

}

void RenderWorld::Shutdown()
{
	logic.cameras.Clear();
	logic.meshlets.Clear();
	logic.pointlights.Clear();

	render.cameras.Clear();
	render.meshlets.Clear();
	render.pointlights.Clear();
}

void RenderWorld::ScreenResize(const unsigned int& width, const unsigned int& height)
{
	for (auto& camera : logic.cameras)
	{
		camera.UpdateView(width, height);
	}
}

void RenderWorld::Tick(float deltaTime)
{
	for (auto& camera : logic.cameras)
	{
		camera.TickComponent(deltaTime);
	}
	
	for (auto& meshlet : logic.meshlets)
	{
		meshlet.TickComponent(deltaTime);
	}
	
	for (auto& pointlight : logic.pointlights)
	{
		pointlight.TickComponent(deltaTime);
	}
}