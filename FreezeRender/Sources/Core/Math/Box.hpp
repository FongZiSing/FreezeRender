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
	public:
		BasicBoundingBox3() noexcept
			: BasicBox3<T>(BasicBox3<T>::Max, BasicBox3<T>::Min)
		{}

		BasicBoundingBox3(const BasicVector3<T>& position) noexcept
			: BasicBox3<T>(position, position)
		{}

		//--------------------------------
		//~ Begin self-related operations.

		warn_nodiscard constexpr bool IsValid() const noexcept;
		warn_nodiscard force_inline BasicVector3<T> GetCenter() const noexcept { return (this->max + this->min) / 2; }
		warn_nodiscard force_inline BasicVector3<T> GetExtent() const noexcept { return (this->max - this->min) / 2; }
		warn_nodiscard force_inline BasicVector3<T> GetSize() const noexcept { return (this->max - this->min); }

		force_inline const BasicBoundingBox3& operator += (const BasicVector3<T>& rhs);
		force_inline const BasicBoundingBox3& operator += (const BasicBoundingBox3& rhs);

		//~ Begin  self-related operations.
		//--------------------------------
	};

	using BoundingBox3i = BasicBoundingBox3<int>;
	using BoundingBox3f = BasicBoundingBox3<float>;
	using BoundingBox3d = BasicBoundingBox3<double>;
	static_assert(sizeof(BoundingBox3i) == 24, "[FreezeRender] BoundingBox2i size is invalid!");
	static_assert(sizeof(BoundingBox3f) == 24, "[FreezeRender] BoundingBox2f size is invalid!");
	static_assert(sizeof(BoundingBox3d) == 48, "[FreezeRender] BoundingBox2d size is invalid!");



#pragma region boundingbox3_implemention

	template<typename T>
	force_inline constexpr bool BasicBoundingBox3<T>::IsValid() const noexcept
	{
		return this->max.x >= this->min.x && this->max.y >= this->min.y && this->max.z >= this->min.z;
	}

	template<typename T>
	force_inline const BasicBoundingBox3<T>& BasicBoundingBox3<T>::operator += (const BasicVector3<T>& rhs)
	{
		this->min.x = std::min(this->min.x, rhs.x);
		this->min.y = std::min(this->min.y, rhs.y);
		this->min.z = std::min(this->min.z, rhs.z);

		this->max.x = std::max(this->max.x, rhs.x);
		this->max.y = std::max(this->max.y, rhs.y);
		this->max.z = std::max(this->max.z, rhs.z);

		return *this;
	}

	template<typename T>
	force_inline const BasicBoundingBox3<T>& BasicBoundingBox3<T>::operator += (const BasicBoundingBox3<T>& rhs)
	{
		this->min.x = std::min(this->min.x, rhs.min.x);
		this->min.y = std::min(this->min.y, rhs.min.y);
		this->min.z = std::min(this->min.z, rhs.min.z);

		this->max.x = std::max(this->max.x, rhs.max.x);
		this->max.y = std::max(this->max.y, rhs.max.y);
		this->max.z = std::max(this->max.z, rhs.max.z);

		return *this;
	}

#pragma endregion boundingbox3_implemention
}