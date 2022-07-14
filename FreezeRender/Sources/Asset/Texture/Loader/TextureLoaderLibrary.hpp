// TextureLoaderLibrary.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Texture loader library.
//
#pragma once

#include "TextureLoader.hpp"



namespace Pluto
{
	struct TextureLoaderLibrary
	{
		/**
		 * @biref Load meshlet according to the specified filepath.
		 */
		static TextureLoader::Status Load(
			const wchar_t* const filepath,
			ATexture* result,
			APixelFormat asformat = APixelFormat::UCHAR_RGBA);
	};
}