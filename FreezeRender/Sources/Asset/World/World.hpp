//
// World.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// World asset.
//
#pragma once

#include <Container/Array.hpp>
#include <Asset/Meshlet/Meshlet.hpp>
#include <Asset/Camera/Camera.hpp>
#include <Asset/Light/Light.hpp>



namespace Pluto
{
	struct AWorld
	{
		Array<ACamera> allCamera;
		Array<AMeshlet> allMeshlet;
		Array<APointLight> allPointLight;
	};
}