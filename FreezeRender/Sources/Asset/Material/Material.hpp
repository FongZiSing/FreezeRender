#pragma once

#include <Asset/Texture/Texture.hpp>



/**
 * @brief Material object.
 */
struct Material
{
	using TextureReference = std::unique_ptr<Texture>;

	WideString id            { L"Native" };
	WideString name          { L"Native" };

	TextureReference diffuse { nullptr };
	TextureReference normal  { nullptr };
};