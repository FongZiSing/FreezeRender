//
// PointLightComponent.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of PointLight component.
//
#pragma once

#include <Render/Shading/Light.hpp>



namespace Pluto
{
	class PointLightComponent
	{
	private:
		APointLight& data;
		PointLight& light;


	public:
		explicit PointLightComponent(APointLight& asset, PointLight& target)
			: data(asset)
			, light(target)
		{}

		void TickComponent(float deltaTime);
	};
}