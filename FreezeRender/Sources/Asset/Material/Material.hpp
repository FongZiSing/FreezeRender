#pragma once

#include <Asset/Texture/Texture.hpp>



/**
 * @brief Material object.
 */
struct AMaterial
{
	using TextureReference = std::unique_ptr<ATexture>;

	WideString id { L"Native" };
	WideString name { L"Native" };

	TextureReference diffuse { nullptr };
	TextureReference normal { nullptr };
};