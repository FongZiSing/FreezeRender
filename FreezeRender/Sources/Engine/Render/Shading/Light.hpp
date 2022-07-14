//
// Light.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of light.
//
#pragma once

#include <Asset/Light/Light.hpp>



namespace Pluto
{
	struct PointLight
	{
		APointLight& data;


		explicit PointLight(APointLight& target)
			: data(target)
		{}
	};
}