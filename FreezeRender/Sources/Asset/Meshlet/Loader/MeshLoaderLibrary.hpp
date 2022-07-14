//
// MeshLoaderLibrary.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Mesh loader library.
//
#pragma once

#include "MeshLoader.hpp"



namespace Pluto
{
	struct MeshLoaderLibrary
	{
		/**
		 * @biref Load meshlet according to the specified filename.
		 */
		static MeshLoader::Status Load(const wchar_t* const filename, AMeshlet* result);
	};
}