#pragma once

#include <Render/ShadingLight.hpp>



class PointLightComponent
{
private:
	PointLight& entity;
	ShadingPointLight light;


public:
	explicit PointLightComponent(PointLight& targetEntity, ShadingPointLight& pointLight)
		: entity(targetEntity)
		, light(pointLight)
	{}

	void TickComponent(float deltaTime);
};