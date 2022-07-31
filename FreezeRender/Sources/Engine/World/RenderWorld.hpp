//
// RenderWorld.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of world.
//
#pragma once

#include <Core/Base/Container/Array.hpp>
#include <Core/Base/Pattern/Singleton.hpp>
#include <Engine/Render/Shading/Light.hpp>
#include <Engine/Component/CameraComponent.hpp>
#include <Engine/Component/MeshletComponent.hpp>
#include <Engine/Component/PointLightComponent.hpp>



namespace Pluto
{
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
}