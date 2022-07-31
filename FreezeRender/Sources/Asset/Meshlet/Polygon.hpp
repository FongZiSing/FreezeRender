//
// Polygon.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of polygon.
//
#pragma once

#include <Core/Math/Matrix.hpp>
#include <Core/Math/Color.hpp>



namespace Pluto
{
	struct AVertex
	{
		Vector3f position;
		Vector3f normal;
		Vector2f uv;
		Color color;
		unsigned int materialIndex = 0;

		inline bool operator == (const AVertex& rhs) const
		{
			return
				(position - rhs.position).LengthSquared() < Number::SMALL_NUMBER &&
				(normal - rhs.normal).LengthSquared() < Number::SMALL_NUMBER &&
				(uv - rhs.uv).LengthSquared() < Number::SMALL_NUMBER &&
				materialIndex == rhs.materialIndex;
		}
	};
}