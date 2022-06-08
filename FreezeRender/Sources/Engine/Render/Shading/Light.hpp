#pragma once

#include <Asset/Light/Light.hpp>



struct PointLight
{
	APointLight& data;


	explicit PointLight(APointLight& target)
		: data(target)
	{}
};