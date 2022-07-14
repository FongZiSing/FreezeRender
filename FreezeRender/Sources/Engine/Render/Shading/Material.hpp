//
// Material.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of material.
//
#pragma once

#include <Asset/Material/Material.hpp>
#include "Texture.hpp"



namespace Pluto
{
	struct Material
	{
		AMaterial& data;

		Texture diffuse;
		Texture normal;


		explicit Material(AMaterial& target)
			: data(target)
			, diffuse(*target.diffuse.get())
			, normal(*target.normal.get())
		{
		}
	};
}