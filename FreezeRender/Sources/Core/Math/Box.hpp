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
		Vector2 min;
		Vector2 max;

		AABB2() noexcept = default;
		
		AABB2(const Vector2& minPosition, const Vector2& maxPosition) noexcept
			: min(minPosition)
			, max(maxPosition)
		{}

		inline Vector2 GetCenter() const noexcept { return (max + min) * 0.5f; }

		inline Vector2 GetExtent() const noexcept { return (max - min) * 0.5f; }
	};



	/**
	 * @brief The implemention of axis-aligned bounding box.
	 * @see https://en.wikipedia.org/wiki/AABB
	 */
	struct AABB3
	{
		Vector3 min;
		Vector3 max;

		AABB3() noexcept = default;

		AABB3(const Vector3& minPosition, const Vector3& maxPosition) noexcept
			: min(minPosition)
			, max(maxPosition)
		{}

		inline Vector3 GetCenter() const noexcept { return (max + min) * 0.5f; }

		inline Vector3 GetExtent() const noexcept { return (max - min) * 0.5f; }
	};
}