#pragma once

#include <Asset/Light/Light.hpp>



struct ShadingPointLight
{
	PointLight& pointLight;


	explicit ShadingPointLight(PointLight& target)
		: pointLight(target)
	{}
};