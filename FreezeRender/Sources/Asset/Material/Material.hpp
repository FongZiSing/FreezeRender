//
// Texture.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Texture asset.
//
#pragma once

#include <Asset/Texture/Texture.hpp>



namespace Pluto
{
	/**
	 * @brief Material object.
	 */
	struct AMaterial
	{
		using TextureReference = std::unique_ptr<ATexture>;

		WideString id{ L"Native" };
		WideString name{ L"Native" };

		TextureReference diffuse{ nullptr };
		TextureReference normal{ nullptr };
	};
}