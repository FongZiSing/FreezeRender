//
// WorldLoaderLibrary.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// World loader library.
//
#pragma once

#include "../World.hpp"



namespace Pluto
{
	struct WorldLoaderLibrary
	{
		static void InitializeDefaultWorld(AWorld& world);
	};
}