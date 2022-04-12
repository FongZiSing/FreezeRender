#pragma once

#include <Container/Array.hpp>
#include <Render/ShadingLight.hpp>
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
		Array<ShadingCamera> cameras;

		Array<ShadingMeshlet> meshlets;

		Array<ShadingPointLight> pointlights;

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

extern RenderWorld* GWorld;