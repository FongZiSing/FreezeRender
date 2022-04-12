#pragma once

#include "MeshLoader.hpp"



struct MeshLoaderLibrary
{
	/**
	 * @biref Load meshlet according to the specified filename.
	 */
	static MeshLoader::Status Load(const wchar_t* const filename, Meshlet* result);
};