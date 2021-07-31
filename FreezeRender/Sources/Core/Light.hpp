#pragma once

#include <Common.hpp>
#include <vector>
#include <memory>
#include "Matrix.hpp"



/**
 * @brief The base light object. It should contains data only and not be used directly.
 */
struct interface_as Light
{
	float intensity;

	Vector3 location;
};



/**
 * @brief The point light object.
 */
class PointLight final : public Light
{
	// TODO
};

