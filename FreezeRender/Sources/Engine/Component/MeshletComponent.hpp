#pragma once

#include <Render/ShadingMeshlet.hpp>



class MeshletComponent
{
private:
	Meshlet& entity;
	ShadingMeshlet& meshlet;


public:
	MeshletComponent(Meshlet& targetEntity, ShadingMeshlet& meshlet)
		: entity(targetEntity)
		, meshlet(meshlet)
	{
	}

	void TickComponent(float deltaTime);
};