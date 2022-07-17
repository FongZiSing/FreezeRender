//
// Polygon.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of polygon.
//
#pragma once

#include <Math/Matrix.hpp>
#include <Math/Color.hpp>



namespace Pluto
{
	struct AVertex
	{
		Vector3f position;
		Vector3f normal;
		Vector2f uv;
		Color color;
	};



	struct AVertexCluster
	{
		unsigned int beginVertexIndex = 0;
		unsigned int endVertexIndex = 0;
		unsigned int triangleNumber = 0;
		unsigned int materialIndex = 0;
	};



	inline bool operator == (const AVertex& lhs, const AVertex& rhs)
	{
		return
			(lhs.position - rhs.position).LengthSquared() < Number::SMALL_NUMBER &&
			(lhs.normal - rhs.normal).LengthSquared() < Number::SMALL_NUMBER &&
			(lhs.uv - rhs.uv).LengthSquared() < Number::SMALL_NUMBER;
	}
}