#pragma once

#include <Common.hpp>
#include <Math/Matrix.hpp>
#include <Math/Rotator.hpp>



/**
 * @brief Camera object.
 */
struct ACamera
{
	enum class Type : unsigned char
	{
		Euler = 0,
		Max,
	};

	Vector3 location      = { 0.f, 0.f, 10.f };
	Rotator rotation      = { 0.f, 0.f, 0.f  };
	int resolutionX       = 0;
	int resolutionY       = 0;
	float aspectRatio     = 0;
	float fieldOfView     = 45.f; // in degrees.
	float nearPlane       = 0.1f;
	float farPlane        = 50.f;
	const float moveSpeed = 0.05f;
};