//
// MeshletComponent.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of meshlet component.
//
#pragma once

#include <Render/Shading/Meshlet.hpp>



namespace Pluto
{
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
}