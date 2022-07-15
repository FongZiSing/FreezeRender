//
// Math/Box.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of bounding box.
//
#pragma once

#include "Matrix.hpp"



namespace Pluto
{
	struct Box2
	{
		int minX, maxX;
		int minY, maxY;
	};



	/**
	 * @brief The implemention of axis-aligned bounding box.
	 * @see https://en.wikipedia.org/wiki/AABB
	 */
	struct AABB2
	{
		Vector2f min;
		Vector2f max;

		AABB2() noexcept = default;
		
		AABB2(const Vector2f& minPosition, const Vector2f& maxPosition) noexcept
			: min(minPosition)
			, max(maxPosition)
		{}

		inline Vector2f GetCenter() const noexcept { return (max + min) * 0.5f; }

		inline Vector2f GetExtent() const noexcept { return (max - min) * 0.5f; }
	};



	/**
	 * @brief The implemention of axis-aligned bounding box.
	 * @see https://en.wikipedia.org/wiki/AABB
	 */
	struct AABB3
	{
		Vector3f min;
		Vector3f max;

		AABB3() noexcept = default;

		AABB3(const Vector3f& minPosition, const Vector3f& maxPosition) noexcept
			: min(minPosition)
			, max(maxPosition)
		{}

		inline Vector3f GetCenter() const noexcept { return (max + min) * 0.5f; }

		inline Vector3f GetExtent() const noexcept { return (max - min) * 0.5f; }
	};
}