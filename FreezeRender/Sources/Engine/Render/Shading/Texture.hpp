//
// Texture.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of texture.
//
#pragma once

#include <Common.hpp>
#include "Sampler.hpp"



namespace Pluto
{
	/**
	 * @brief Implemention of texture.
	 */
	struct Texture
	{
		const ATexture& data;
		const TextureSampler sampler;


		Texture(const ATexture& target)
			: data(target)
			, sampler(&target)
		{}
	};
}