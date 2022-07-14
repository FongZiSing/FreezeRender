//
// Light.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Light asset.
//
#pragma once

#include <Common.hpp>
#include <Math/Matrix.hpp>



namespace Pluto
{
	/**
	 * @brief The light object.
	 */
	struct ALight
	{
		float intensity;

		Vector3 location;
	};



	/**
	 * @brief The point light object.
	 */
	struct APointLight final : public ALight
	{
		// TODO
	};
}
