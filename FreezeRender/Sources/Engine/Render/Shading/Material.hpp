#pragma once

#include <Asset/Material/Material.hpp>
#include "Texture.hpp"



struct Material
{
	AMaterial& data;

	Texture diffuse;
	Texture normal;


	explicit Material(AMaterial& target)
		: data(target)
		, diffuse(target.diffuse.get())
		, normal(target.normal.get())
	{
	}
};