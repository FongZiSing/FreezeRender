#include "TextureLoaderLibrary.hpp"
#include "WICTextureLoader.hpp"



TextureLoader::Status TextureLoaderLibrary::Load(const wchar_t* const filepath, Texture* result, APixelFormat asformat)
{
	WICTextureLoader loader(filepath);
	return loader.Load(result, asformat);
}