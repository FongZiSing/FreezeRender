#pragma once

#include <Container/Array.hpp>
#include <Asset/Meshlet/Meshlet.hpp>
#include <Asset/Camera/Camera.hpp>
#include <Asset/Light/Light.hpp>



struct World
{
	Array<Camera> allCamera;
	Array<Meshlet> allMeshlet;
	Array<PointLight> allPointLight;
};