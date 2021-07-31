#pragma once

#include "TextureLoader.hpp"



struct TextureLoaderLibrary
{
	/**
	 * @biref Load meshlet according to the specified filepath.
	 */
	static TextureLoader::Status Load(const wchar_t* const filepath, Texture* result, APixelFormat asformat = APixelFormat::UCHAR_RGBA);
};