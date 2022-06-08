#pragma once

#include <Container/Array.hpp>
#include <Pattern/Singleton.hpp>
#include <Render/Shading/Light.hpp>
#include <Component/CameraComponent.hpp>
#include <Component/MeshletComponent.hpp>
#include <Component/PointLightComponent.hpp>



class RenderWorld
{
private:
	friend class Engine;

public:

	struct
	{
		Array<Camera> cameras;

		Array<Meshlet> meshlets;

		Array<PointLight> pointlights;

	} render;
	

	struct
	{
		Array<CameraComponent> cameras;

		Array<MeshletComponent> meshlets;

		Array<PointLightComponent> pointlights;

	} logic;



	RenderWorld();

	~RenderWorld();

	void Load();

	void Unload() {}

private:
	void Startup(unsigned int screenWidth, unsigned int screenHeight);

	void Shutdown();

	void ScreenResize(const unsigned int& width, const unsigned int& height);

	void Tick(float deltaTime);
};

extern UniqueResource<RenderWorld> GWorld;