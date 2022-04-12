#pragma once

#include <Common.hpp>
#include <Math/Matrix.hpp>



/**
 * @brief The light object.
 */
struct Light
{
	float intensity;

	Vector3 location;
};



/**
 * @brief The point light object.
 */
struct PointLight final : public Light
{
	// TODO
};
