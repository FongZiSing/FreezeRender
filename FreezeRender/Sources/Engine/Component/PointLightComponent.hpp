#pragma once

#include <Render/Shading/Light.hpp>



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