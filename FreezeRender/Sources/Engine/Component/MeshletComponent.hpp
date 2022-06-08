#pragma once

#include <Render/Shading/Meshlet.hpp>



class MeshletComponent
{
private:
	AMeshlet& data;
	Meshlet& meshlet;


public:
	MeshletComponent(AMeshlet& asset, Meshlet& target)
		: data(asset)
		, meshlet(target)
	{
	}

	void TickComponent(float deltaTime);
};