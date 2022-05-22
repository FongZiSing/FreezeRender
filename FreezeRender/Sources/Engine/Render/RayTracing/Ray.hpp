#pragma once

#include <Core/Math/Matrix.hpp>
#include "../RenderTarget.hpp"



struct Ray
{
	// Ray = origin + t * direction;
	Vector3 origin;
	Vector3 direction;
	float t;
};


struct RayPixelTraits
{
	using Type = Ray;
	inline static const Type defaultPixelValue = {};
}; typedef RenderTarget<RayPixelTraits> PrimitiveRay;



