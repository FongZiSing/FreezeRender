#pragma once

#include <Container/Array.hpp>
#include <Asset/Meshlet/Meshlet.hpp>
#include <Asset/Camera/Camera.hpp>
#include <Asset/Light/Light.hpp>



struct AWorld
{
	Array<ACamera> allCamera;
	Array<AMeshlet> allMeshlet;
	Array<APointLight> allPointLight;
};