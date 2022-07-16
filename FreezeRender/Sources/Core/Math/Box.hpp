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
	/**
	 * @brief Templated box2.
	 */
	template<typename T>
	struct alignas(16) BasicBox2
	{
		BasicVector2<T> min;
		BasicVector2<T> max;
	};

	using Box2i = BasicBox2<int>;
	using Box2f = BasicBox2<float>;
	using Box2d = BasicBox2<double>;
	static_assert(sizeof(Box2i) == 16, "[FreezeRender] Box2i size is invalid!");
	static_assert(sizeof(Box2f) == 16, "[FreezeRender] Box2f size is invalid!");
	static_assert(sizeof(Box2d) == 32, "[FreezeRender] Box2d size is invalid!");



	/**
	 * @brief Templated box3.
	 */
	template<typename T>
	struct BasicBox3
	{
		BasicVector3<T> min;
		BasicVector3<T> max;
	};

	using Box3i = BasicBox3<int>;
	using Box3f = BasicBox3<float>;
	using Box3d = BasicBox3<double>;
	static_assert(sizeof(Box3i) == 24, "[FreezeRender] Box3i size is invalid!");
	static_assert(sizeof(Box3f) == 24, "[FreezeRender] Box3f size is invalid!");
	static_assert(sizeof(Box3d) == 48, "[FreezeRender] Box3d size is invalid!");



	/**
	 * @brief Templated axis-aligned bounding box.
	 * @see https://en.wikipedia.org/wiki/AABB
	 */
	template <typename T>
	struct BasicBoundingBox3 : public BasicBox3<T>
	{
	private:
		 bool bIsValid = false;


	public:
		explicit BasicBoundingBox3(const BasicVector3<T>& minPosition, const BasicVector3<T>& maxPosition) noexcept
			: BasicBox3<T>(minPosition, maxPosition)
			, bIsValid(true)
		{}

		//--------------------------------
		//~ Begin self-related operations.

		constexpr bool IsValid() const noexcept { return bIsValid; }
		force_inline BasicVector3<T> GetCenter() const noexcept { return (this->max + this->min) / 2; }
		force_inline BasicVector3<T> GetExtent() const noexcept { return (this->max - this->min) / 2; }
		
		force_inline const BasicBoundingBox3& operator += (const BasicBoundingBox3& rhs);

		//~ Begin  self-related operations.
		//--------------------------------
	};

	using BoundingBox3i = BasicBoundingBox3<int>;
	using BoundingBox3f = BasicBoundingBox3<float>;
	using BoundingBox3d = BasicBoundingBox3<double>;
	static_assert(sizeof(BoundingBox3i) == 28, "[FreezeRender] BoundingBox2i size is invalid!");
	static_assert(sizeof(BoundingBox3f) == 28, "[FreezeRender] BoundingBox2f size is invalid!");
	static_assert(sizeof(BoundingBox3d) == 56, "[FreezeRender] BoundingBox2d size is invalid!");



#pragma region boundingbox3_implemention

	template<typename T>
	force_inline const BasicBoundingBox3<T>& BasicBoundingBox3<T>::operator += (const BasicBoundingBox3<T>& rhs)
	{
		if (bIsValid && rhs.bIsValid) branch_likely
		{
			this->min.x = std::min(this->min.x, rhs.min.x);
			this->min.y = std::min(this->min.y, rhs.min.y);
			this->min.z = std::min(this->min.z, rhs.min.z);

			this->max.x = std::max(this->max.x, rhs.max.x);
			this->max.y = std::max(this->max.y, rhs.max.y);
			this->max.z = std::max(this->max.z, rhs.max.z);
		}
		else if (rhs.bIsValid) branch_unlikely
		{
			*this = rhs;
		}

		return *this;
	}

#pragma endregion boundingbox3_implemention
}