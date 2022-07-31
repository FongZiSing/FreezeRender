//
// Camera.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Camera asset.
//
#pragma once

#include <Common.hpp>
#include <Core/Math/Matrix.hpp>
#include <Core/Math/Rotator.hpp>



namespace Pluto
{
	/**
	 * @brief Camera asset.
	 */
	struct ACamera
	{
		enum class Type : unsigned char
		{
			Euler = 0,
			Max,
		};

		Vector3f location     = { 0.f, 0.f, 10.f };
		Rotator rotation      = { 0.f, 0.f, 0.f  };
		int resolutionX       = 0;
		int resolutionY       = 0;
		float aspectRatio     = 0;
		float fieldOfView     = 45.f; // in degrees.
		float nearPlane       = 0.1f;
		float farPlane        = 50.f;
		const float moveSpeed = 0.05f;
	};
}