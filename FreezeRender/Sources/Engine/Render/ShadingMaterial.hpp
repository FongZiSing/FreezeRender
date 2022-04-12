#pragma once

#include <Asset/Material/Material.hpp>
#include "ShadingTexture.hpp"



struct ShadingMaterial
{
	Material& material;
	
	ShadingTexture diffuse;
	ShadingTexture normal;


	explicit ShadingMaterial(Material& target)
		: material(target)
		, diffuse(target.diffuse.get())
		, normal(target.normal.get())
	{
	}
};