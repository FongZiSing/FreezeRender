//
// Math/Matrix.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of vector and matrix.
//
#pragma once

#include <Common.hpp>
#include <concepts>
#include "Math.hpp"
#include "Number.hpp"



namespace Pluto
{
	template <typename T> concept is_floating = std::is_same_v<T, float> || std::is_same_v<T, double>;
	template <typename T> concept is_integral = std::is_same_v<T, int>;
	template<typename T> struct BasicMatrix;



	/**
	 * @brief Templated vector2.
	 */
	template <typename T>
	struct BasicVector2
	{
		static_assert(is_floating<T> || is_integral<T>, "[FreezeRender] type must be int, float, or double");
		
		T x, y;


		//--------------------------------
		//~ Begin constants.

		static const BasicVector2 Zero;
		static const BasicVector2 One;

		//~ End constants.
		//--------------------------------


		//--------------------------------
		//~ Begin initialize.

		BasicVector2() noexcept : x(0), y(0) {}
		BasicVector2(const T& inValue) noexcept : x(inValue), y(inValue) {}
		BasicVector2(const T& inX, const T& inY) noexcept : x(inX), y(inY) {}
		BasicVector2(const BasicVector2& inValue) : x(inValue.x), y(inValue.y) {}
		force_inline BasicVector2& operator = (const BasicVector2& inValue) noexcept;

		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin self-related operations.

		force_inline bool operator == (const BasicVector2& rhs) const;
		force_inline bool operator != (const BasicVector2& rhs) const;

		force_inline BasicVector2 operator + (const BasicVector2& rhs) const;
		force_inline BasicVector2 operator - (const BasicVector2& rhs) const;
		force_inline BasicVector2 operator * (const BasicVector2& rhs) const;
		force_inline BasicVector2 operator / (const BasicVector2& rhs) const;
		force_inline const BasicVector2& operator += (const BasicVector2& rhs);
		force_inline const BasicVector2& operator -= (const BasicVector2& rhs);
		force_inline const BasicVector2& operator *= (const BasicVector2& rhs);
		force_inline const BasicVector2& operator /= (const BasicVector2& rhs);

		//~ End self-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin number-related operations.

		force_inline BasicVector2 operator + (const T& rhs) const;
		force_inline BasicVector2 operator - (const T& rhs) const;
		force_inline BasicVector2 operator * (const T& rhs) const;
		force_inline BasicVector2 operator / (const T& rhs) const;
		force_inline const BasicVector2& operator += (const T& rhs);
		force_inline const BasicVector2& operator -= (const T& rhs);
		force_inline const BasicVector2& operator *= (const T& rhs);
		force_inline const BasicVector2& operator /= (const T& rhs);

		//~ End number-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin bector operations.

		force_inline T operator ^ (const BasicVector2& rhs) const;
		force_inline T operator | (const BasicVector2& rhs) const;
		warn_nodiscard force_inline T CrossProduct(const BasicVector2& rhs) const;
		warn_nodiscard force_inline T DotProduct(const BasicVector2& rhs) const;
		
		warn_nodiscard force_inline T Length() const requires is_floating<T>;
		warn_nodiscard force_inline T LengthSquared() const requires is_floating<T>;
		warn_nodiscard force_inline BasicVector2 Normalize(const float& tolerance = Number::SMALL_NUMBER) const requires is_floating<T>;
		force_inline void Normalized(const float& tolerance = Number::SMALL_NUMBER) requires is_floating<T>;
		warn_nodiscard force_inline bool IsNormalized(const float& tolerance = Number::SMALL_NUMBER) const requires is_floating<T>;

		//~ End vector operations.
		//--------------------------------

		warn_nodiscard force_inline BasicMatrix<T> ToMatrix() const;
		warn_nodiscard force_inline BasicMatrix<T> ToInvMatrix() const;
	};

	using Vector2i = BasicVector2<int>;
	using Vector2f = BasicVector2<float>;
	using Vector2d = BasicVector2<double>;
	static_assert(sizeof(Vector2i) == 8, "[FreezeRender] Vector2i size is invalid!");
	static_assert(sizeof(Vector2f) == 8, "[FreezeRender] Vector2f size is invalid!");
	static_assert(sizeof(Vector2d) == 16, "[FreezeRender] Vector2d size is invalid!");



	/**
	 * @brief Templated vector3.
	 */
	template <typename T>
	struct BasicVector3
	{
		static_assert(is_floating<T> || is_integral<T>, "[FreezeRender] type must be int, float, or double");
		
		T x, y, z;


		//--------------------------------
		//~ Begin constants.

		static const BasicVector3 Zero;
		static const BasicVector3 One;

		//~ End constants.
		//--------------------------------


		//--------------------------------
		//~ Begin initialize.

		BasicVector3() noexcept : x(0), y(0), z(0) {}
		BasicVector3(const T& inValue) noexcept : x(inValue), y(inValue), z(inValue) {}
		BasicVector3(const T& inX, const T& inY, const T& inZ) noexcept : x(inX), y(inY), z(inZ) {}
		BasicVector3(const BasicVector2<T>& inValue, const T& inZ = 0) : x(inValue.x), y(inValue.y), z(inZ) {}
		BasicVector3(const T& inX, const BasicVector2<T>& inValue) : x(inX), y(inValue.x), z(inValue.y) {}
		BasicVector3(const BasicVector3& inValue) : x(inValue.x), y(inValue.y), z(inValue.z) {}
		force_inline BasicVector3& operator = (const BasicVector3& inValue) noexcept;
	
		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin self-related operations.

		force_inline BasicVector2<T> XY();
		force_inline BasicVector2<T> XZ();
		force_inline BasicVector2<T> YZ();
		force_inline BasicVector2<T>& XYRef();
		force_inline BasicVector2<T>& YZRef();

		force_inline bool operator == (const BasicVector3& rhs) const;
		force_inline bool operator != (const BasicVector3& rhs) const;

		force_inline BasicVector3 operator + (const BasicVector3& rhs) const;
		force_inline BasicVector3 operator - (const BasicVector3& rhs) const;
		force_inline BasicVector3 operator * (const BasicVector3& rhs) const;
		force_inline BasicVector3 operator / (const BasicVector3& rhs) const;
		force_inline const BasicVector3& operator += (const BasicVector3& rhs);
		force_inline const BasicVector3& operator -= (const BasicVector3& rhs);
		force_inline const BasicVector3& operator *= (const BasicVector3& rhs);
		force_inline const BasicVector3& operator /= (const BasicVector3& rhs);

		//~ End self-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin number-related operations.

		force_inline BasicVector3 operator + (const T& rhs) const;
		force_inline BasicVector3 operator - (const T& rhs) const;
		force_inline BasicVector3 operator * (const T& rhs) const;
		force_inline BasicVector3 operator / (const T& rhs) const;
		force_inline const BasicVector3& operator += (const T& rhs);
		force_inline const BasicVector3& operator -= (const T& rhs);
		force_inline const BasicVector3& operator *= (const T& rhs);
		force_inline const BasicVector3& operator /= (const T& rhs);

		//~ End number-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin vector operations.

		force_inline BasicVector3 operator ^ (const BasicVector3& rhs) const;
		force_inline T operator | (const BasicVector3& rhs) const;
		force_inline void CrossProducted(const BasicVector3& rhs);
		warn_nodiscard force_inline BasicVector3 CrossProduct(const BasicVector3& rhs) const;
		warn_nodiscard force_inline T DotProduct(const BasicVector3& rhs) const;
		
		warn_nodiscard force_inline T Length() const requires is_floating<T>;
		warn_nodiscard force_inline T LengthSquared() const requires is_floating<T>;
		warn_nodiscard force_inline BasicVector3 Normalize(const float& tolerance = Number::SMALL_NUMBER) const requires is_floating<T>;
		force_inline void Normalized(const float& tolerance = Number::SMALL_NUMBER) requires is_floating<T>;
		warn_nodiscard force_inline bool IsNormalized(const float& tolerance = Number::SMALL_NUMBER) const requires is_floating<T>;

		//~ End vector operations.
		//--------------------------------

		warn_nodiscard force_inline BasicMatrix<T> ToMatrix() const;
		warn_nodiscard force_inline BasicMatrix<T> ToInvMatrix() const;
	};

	using Vector3i = BasicVector3<int>;
	using Vector3f = BasicVector3<float>;
	using Vector3d = BasicVector3<double>;
	static_assert(sizeof(Vector3i) == 12, "[FreezeRender] Vector3i size is invalid!");
	static_assert(sizeof(Vector3f) == 12, "[FreezeRender] Vector3f size is invalid!");
	static_assert(sizeof(Vector3d) == 24, "[FreezeRender] Vector3d size is invalid!");



	/**
	 * @brief Templated vector4.
	 */
	template <typename T>
	struct alignas(16) BasicVector4
	{
		static_assert(is_floating<T> || is_integral<T>, "[FreezeRender] type must be int, float, or double");
		T x, y, z, w;


		//--------------------------------
		//~ Begin constants.

		static const BasicVector4 Zero;
		static const BasicVector4 One;

		//~ End constants.
		//--------------------------------


		//--------------------------------
		//~ Begin initialize.

		BasicVector4() noexcept : x(0), y(0), z(0), w(0) {}
		BasicVector4(const T& inValue) noexcept : x(inValue), y(inValue), z(inValue), w(inValue) {}
		BasicVector4(const T& inX, const T& inY, const T& inZ, const T& inW) noexcept : x(inX), y(inY), z(inZ), w(inW) {}
		BasicVector4(const BasicVector2<T>& inValue, const T& inZ = 0, const T& inW = 0) : x(inValue.x), y(inValue.y), z(inZ), w(inW) {}
		BasicVector4(const T& inX, const T& inY, const BasicVector2<T>& inValue) : x(inX), y(inY), z(inValue.x), w(inValue.y) {}
		BasicVector4(const BasicVector2<T>& in0, const BasicVector2<T>& in1) : x(in0.x), y(in0.y), z(in1.x), w(in1.y) {}
		BasicVector4(const T& inX, const BasicVector3<T>& inValue) : x(inX), y(inValue.x), z(inValue.y), w(inValue.z) {}
		BasicVector4(const BasicVector3<T>& inValue, const T& inW = 0) : x(inValue.x), y(inValue.y), z(inValue.z), w(inW) {}
		BasicVector4(const BasicVector4& inValue) : x(inValue.x), y(inValue.y), z(inValue.z), w(inValue.w) {}
		force_inline BasicVector4& operator = (const BasicVector4& inValue) noexcept;

		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin self-related operations.

		force_inline BasicVector2<T> XY() const;
		force_inline BasicVector2<T> ZW() const;
		force_inline BasicVector3<T> XYZ() const;
		force_inline BasicVector3<T> YZW() const;
		force_inline BasicVector2<T>& XYRef() const;
		force_inline BasicVector2<T>& ZWRef() const;
		force_inline BasicVector3<T>& XYZRef() const;
		force_inline BasicVector3<T>& YZWRef() const;

		force_inline bool operator == (const BasicVector4& rhs) const;
		force_inline bool operator != (const BasicVector4& rhs) const;

		force_inline BasicVector4 operator + (const BasicVector4& rhs) const;
		force_inline BasicVector4 operator - (const BasicVector4& rhs) const;
		force_inline BasicVector4 operator * (const BasicVector4& rhs) const;
		force_inline BasicVector4 operator / (const BasicVector4& rhs) const;
		force_inline const BasicVector4& operator += (const BasicVector4& rhs);
		force_inline const BasicVector4& operator -= (const BasicVector4& rhs);
		force_inline const BasicVector4& operator *= (const BasicVector4& rhs);
		force_inline const BasicVector4& operator /= (const BasicVector4& rhs);
	
		//~ End self-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin number-related operations.

		force_inline BasicVector4 operator + (const T& rhs) const;
		force_inline BasicVector4 operator - (const T& rhs) const;
		force_inline BasicVector4 operator * (const T& rhs) const;
		force_inline BasicVector4 operator / (const T& rhs) const;
		force_inline const BasicVector4& operator += (const T& rhs);
		force_inline const BasicVector4& operator -= (const T& rhs);
		force_inline const BasicVector4& operator *= (const T& rhs);
		force_inline const BasicVector4& operator /= (const T& rhs);

		//~ End number-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin vector operations.

		force_inline T operator | (const BasicVector4& rhs) const;
		force_inline T DotProduct(const BasicVector4& rhs) const;

		warn_nodiscard force_inline T Length() const requires is_floating<T>;
		warn_nodiscard force_inline T LengthSquared() const requires is_floating<T>;
		warn_nodiscard force_inline BasicVector4 Normalize(const float& tolerance = Number::SMALL_NUMBER) const requires is_floating<T>;
		force_inline void Normalized(const float& tolerance = Number::SMALL_NUMBER) requires is_floating<T>;
		warn_nodiscard force_inline bool IsNormalized(const float& tolerance = Number::SMALL_NUMBER) const requires is_floating<T>;

		//~ End bector operations.
		//--------------------------------
	};

	using Vector4i = BasicVector4<int>;
	using Vector4f = BasicVector4<float>;
	using Vector4d = BasicVector4<double>;
	static_assert(sizeof(Vector4i) == sizeof(R128i) && sizeof(Vector4i) == 16, "[FreezeRender] Vector4i size is invalid!");
	static_assert(sizeof(Vector4f) == sizeof(R128)  && sizeof(Vector4f) == 16, "[FreezeRender] Vector4f size is invalid!");
	static_assert(sizeof(Vector4d) == sizeof(R256)  && sizeof(Vector4d) == 32, "[FreezeRender] Vector4d size is invalid!");



	/**
	 * @brief Templated matrix.
	 */
	template<typename T>
	struct BasicMatrix
	{
		union
		{
			alignas(16) T m[4][4];
		};


		//--------------------------------
		//~ Begin constants.

		static const BasicMatrix Identity;

		//~ End constants.
		//--------------------------------


		//--------------------------------
		//~ Begin initialize.

		BasicMatrix() = default;
		force_inline BasicMatrix(const T& value) noexcept;
		force_inline BasicMatrix(const BasicVector3<T>& row0, const BasicVector3<T>& row1, const BasicVector3<T>& row2, const BasicVector3<T>& row3);
		force_inline BasicMatrix(const BasicVector4<T>& row0, const BasicVector4<T>& row1, const BasicVector4<T>& row2, const BasicVector4<T>& row3);
		force_inline BasicMatrix(const BasicMatrix& value);
		force_inline BasicMatrix& operator = (const BasicMatrix& value);
		force_inline BasicMatrix(const BasicMatrix&& value) noexcept;
		force_inline BasicMatrix& operator = (const BasicMatrix&& value) noexcept;
		
		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin self-related operations.

		force_inline bool operator == (const BasicMatrix& rhs) const;
		force_inline bool operator != (const BasicMatrix& rhs) const;

		force_inline BasicMatrix operator + (const BasicMatrix& rhs) const;
		force_inline BasicMatrix operator - (const BasicMatrix& rhs) const;
		force_inline BasicMatrix operator * (const BasicMatrix& rhs) const requires is_floating<T>;
		force_inline const BasicMatrix& operator += (const BasicMatrix& rhs);
		force_inline const BasicMatrix& operator -= (const BasicMatrix& rhs);
		force_inline const BasicMatrix& operator *= (const BasicMatrix& rhs) requires is_floating<T>;

		warn_nodiscard force_inline BasicMatrix Transpose() const;
		warn_nodiscard force_inline BasicMatrix Inverse() const requires is_floating<T>;

		//~ End self-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin number-related operations.
	
		force_inline BasicMatrix operator * (const T& rhs) const;
		force_inline BasicMatrix operator / (const T& rhs) const;
		force_inline const BasicMatrix& operator *= (const T& rhs);
		force_inline const BasicMatrix& operator /= (const T& rhs);
	
		//~ End number-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin vector operations.

		warn_deprecated("** Performance issues **") force_inline BasicVector3<T> operator * (const BasicVector3<T>& rhs) const requires is_floating<T>;
		warn_deprecated("** Performance issues **") force_inline BasicVector4<T> operator * (const BasicVector4<T>& rhs) const requires is_floating<T>;

		//~ End vector operations.
		//--------------------------------
	};

	using Matrix44i = BasicMatrix<int>;
	using Matrix44f = BasicMatrix<float>;
	using Matrix44d = BasicMatrix<double>;
	static_assert(sizeof(Matrix44i) == sizeof(R128i) * 4 && sizeof(Matrix44i) == 64, "[FreezeRender] matrix44i size is invalid!");
	static_assert(sizeof(Matrix44f) == sizeof(R128) * 4  && sizeof(Matrix44f) == 64, "[FreezeRender] matrix44f size is invalid!");
	static_assert(sizeof(Matrix44d) == sizeof(R256) * 4  && sizeof(Matrix44d) == 128, "[FreezeRender] matrix44d size is invalid!");


#pragma region contants

	const inline_variable Vector2i Vector2i::Zero = {};
	const inline_variable Vector2i Vector2i::One = { 1, 1 };

	const inline_variable Vector2f Vector2f::Zero = {};
	const inline_variable Vector2f Vector2f::One  = { 1.f, 1.f };

	const inline_variable Vector2d Vector2d::Zero = {};
	const inline_variable Vector2d Vector2d::One = { 1.0, 1.0 };



	const inline_variable Vector3i Vector3i::Zero = {};
	const inline_variable Vector3i Vector3i::One = { 1, 1, 1 };

	const inline_variable Vector3f Vector3f::Zero = {};
	const inline_variable Vector3f Vector3f::One  = { 1.f, 1.f, 1.f };

	const inline_variable Vector3d Vector3d::Zero = {};
	const inline_variable Vector3d Vector3d::One = { 1.0, 1.0, 1.0 };



	const inline_variable Vector4i Vector4i::Zero = {};
	const inline_variable Vector4i Vector4i::One = { 1, 1, 1, 1 };

	const inline_variable Vector4f Vector4f::Zero = {};
	const inline_variable Vector4f Vector4f::One  = { 1.f, 1.f, 1.f, 1.f };

	const inline_variable Vector4d Vector4d::Zero = {};
	const inline_variable Vector4d Vector4d::One = { 1.0, 1.0, 1.0, 1.0 };


	const inline_variable Matrix44i Matrix44i::Identity = 1;

	const inline_variable Matrix44f Matrix44f::Identity = 1.f;

	const inline_variable Matrix44d Matrix44d::Identity = 1.0;

#pragma endregion contants



#pragma region vector2_implemention

	template <typename T> force_inline BasicVector2<T>& BasicVector2<T>::operator = (const BasicVector2<T>& inValue) noexcept { x = inValue.x; y = inValue.y; return *this; }

	template <typename T> force_inline bool BasicVector2<T>::operator == (const BasicVector2<T>& rhs) const { return x == rhs.x && y == rhs.y; }
	template <typename T> force_inline bool BasicVector2<T>::operator != (const BasicVector2<T>& rhs) const { return x != rhs.x || y != rhs.y; }

	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator + (const BasicVector2<T>& rhs) const { return BasicVector2<T>(x + rhs.x, y + rhs.y); }
	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator - (const BasicVector2<T>& rhs) const { return BasicVector2<T>(x - rhs.x, y - rhs.y); }
	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator * (const BasicVector2<T>& rhs) const { return BasicVector2<T>(x * rhs.x, y * rhs.y); }
	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator / (const BasicVector2<T>& rhs) const { return BasicVector2<T>(x / rhs.x, y / rhs.y); }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator += (const BasicVector2<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator -= (const BasicVector2<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator *= (const BasicVector2<T>& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator /= (const BasicVector2<T>& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator + (const T& rhs) const { return BasicVector2<T>(x + rhs, y + rhs); }
	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator - (const T& rhs) const { return BasicVector2<T>(x - rhs, y - rhs); }
	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator * (const T& rhs) const { return BasicVector2<T>(x * rhs, y * rhs); }
	template <typename T> force_inline BasicVector2<T> BasicVector2<T>::operator / (const T& rhs) const { return BasicVector2<T>(x / rhs, y / rhs); }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator += (const T& rhs) { x += rhs; y += rhs; return *this; }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator -= (const T& rhs) { x -= rhs; y -= rhs; return *this; }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator *= (const T& rhs) { x *= rhs; y *= rhs; return *this; }
	template <typename T> force_inline const BasicVector2<T>& BasicVector2<T>::operator /= (const T& rhs) { x /= rhs; y /= rhs; return *this; }

	template <typename T> force_inline T BasicVector2<T>::operator ^ (const BasicVector2<T>& rhs) const { return x * rhs.y - y * rhs.x; }
	template <typename T> force_inline T BasicVector2<T>::operator | (const BasicVector2<T>& rhs) const { return x * rhs.x + y * rhs.y; }
	template <typename T> force_inline T BasicVector2<T>::CrossProduct(const BasicVector2<T>& rhs) const { return (*this) ^ rhs; }
	template <typename T> force_inline T BasicVector2<T>::DotProduct(const BasicVector2<T>& rhs) const { return (*this) | rhs; }
	template <typename T> force_inline T BasicVector2<T>::Length() const requires is_floating<T> { return std::sqrt(x * x + y * y); }
	template <typename T> force_inline T BasicVector2<T>::LengthSquared() const requires is_floating<T> { return x * x + y * y; }

	template <typename T>
	force_inline BasicVector2<T> BasicVector2<T>::Normalize(const float& tolerance) const requires is_floating<T>
	{
		BasicVector2<T> result = *this;
		const T squareSum = result.x * result.x + result.y * result.y;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			result.x *= scale;
			result.y *= scale;
		}
		return result;
	}

	template <typename T>
	force_inline void BasicVector2<T>::Normalized(const float& tolerance) requires is_floating<T>
	{
		const T squareSum = x * x + y * y;
		if (squareSum > tolerance)
		{
			const T scale = Math::InvSqrt(squareSum);
			x *= scale;
			y *= scale;
		}
	}

	template <typename T>
	force_inline bool BasicVector2<T>::IsNormalized(const float& tolerance) const requires is_floating<T>
	{
		const T squareSum = x * x + y * y;
		return (std::abs(static_cast<T>(1) - squareSum) < tolerance);
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicVector2<T>::ToMatrix() const
	{
		return BasicMatrix<T> {
			{ 1, 0, 0, x },
			{ 0, 1, 0, y },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 },
		};
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicVector2<T>::ToInvMatrix() const
	{
		return BasicMatrix<T> {
			{ 1, 0, 0, -x },
			{ 0, 1, 0, -y },
			{ 0, 0, 1, 0  },
			{ 0, 0, 0, 1  },
		};
	}

#pragma endregion vector2_implemention



#pragma region vector3_implemention

	template <typename T> force_inline BasicVector3<T>& BasicVector3<T>::operator = (const BasicVector3<T>& inValue) noexcept { x = inValue.x; y = inValue.y; z = inValue.z; return *this; }

	template <typename T> force_inline BasicVector2<T> BasicVector3<T>::XY() { return BasicVector2<T>(x, y); }
	template <typename T> force_inline BasicVector2<T> BasicVector3<T>::XZ() { return BasicVector2<T>(x, z); }
	template <typename T> force_inline BasicVector2<T> BasicVector3<T>::YZ() { return BasicVector2<T>(y, z); }
	template <typename T> force_inline BasicVector2<T>& BasicVector3<T>::XYRef() { return *(BasicVector2<T>*)(&x); }
	template <typename T> force_inline BasicVector2<T>& BasicVector3<T>::YZRef() { return *(BasicVector2<T>*)(&y); }

	template <typename T> force_inline bool BasicVector3<T>::operator == (const BasicVector3<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	template <typename T> force_inline bool BasicVector3<T>::operator != (const BasicVector3<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator + (const BasicVector3<T>& rhs) const { return BasicVector3<T>(x + rhs.x, y + rhs.y, z + rhs.z); }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator - (const BasicVector3<T>& rhs) const { return BasicVector3<T>(x - rhs.x, y - rhs.y, z - rhs.z); }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator * (const BasicVector3<T>& rhs) const { return BasicVector3<T>(x * rhs.x, y * rhs.y, z * rhs.z); }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator / (const BasicVector3<T>& rhs) const { return BasicVector3<T>(x / rhs.x, y / rhs.y, z / rhs.z); }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator += (const BasicVector3<T>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator -= (const BasicVector3<T>& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator *= (const BasicVector3<T>& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator /= (const BasicVector3<T>& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator + (const T& rhs) const { return BasicVector3<T>(x + rhs, y + rhs, z + rhs); }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator - (const T& rhs) const { return BasicVector3<T>(x - rhs, y - rhs, z - rhs); }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator * (const T& rhs) const { return BasicVector3<T>(x * rhs, y * rhs, z * rhs); }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator / (const T& rhs) const { return BasicVector3<T>(x / rhs, y / rhs, z / rhs); }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator += (const T& rhs) { x += rhs; y += rhs; z += rhs; return *this; }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator -= (const T& rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator *= (const T& rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
	template <typename T> force_inline const BasicVector3<T>& BasicVector3<T>::operator /= (const T& rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::operator ^ (const BasicVector3<T>& rhs) const { return BasicVector3<T>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }
	template <typename T> force_inline T BasicVector3<T>::operator | (const BasicVector3<T>& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
	template <typename T> force_inline void BasicVector3<T>::CrossProducted(const BasicVector3<T>& rhs) { (*this) = (*this) ^ rhs; }
	template <typename T> force_inline BasicVector3<T> BasicVector3<T>::CrossProduct(const BasicVector3<T>& rhs) const { return (*this) ^ rhs; }
	template <typename T> force_inline T BasicVector3<T>::DotProduct(const BasicVector3<T>& rhs) const { return (*this) | rhs; }
	template <typename T> force_inline T BasicVector3<T>::Length() const requires is_floating<T> { return std::sqrtf(x * x + y * y + z * z); }
	template <typename T> force_inline T BasicVector3<T>::LengthSquared() const requires is_floating<T> { return x * x + y * y + z * z; }

	template <typename T>
	force_inline BasicVector3<T> BasicVector3<T>::Normalize(const float& tolerance) const requires is_floating<T>
	{
		BasicVector3<T> result = *this;
		const T squareSum = result.x * result.x + result.y * result.y + result.z * result.z;
		if (squareSum > tolerance)
		{
			const T scale = Math::InvSqrt(squareSum);
			result.x *= scale;
			result.y *= scale;
			result.z *= scale;
		}
		return result;
	}

	template <typename T>
	force_inline void BasicVector3<T>::Normalized(const float& tolerance) requires is_floating<T>
	{
		const T squareSum = x * x + y * y + z * z;
		if (squareSum > tolerance)
		{
			const T scale = Math::InvSqrt(squareSum);
			x *= scale;
			y *= scale;
			z *= scale;
		}
	}

	template <typename T>
	force_inline bool BasicVector3<T>::IsNormalized(const float& tolerance) const requires is_floating<T>
	{
		const T squareSum = x * x + y * y + z * z;
		return (std::abs(static_cast<T>(1) - squareSum) < tolerance);
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicVector3<T>::ToMatrix() const
	{
		return BasicMatrix<T>{
			{ 1, 0, 0, x },
			{ 0, 1, 0, y },
			{ 0, 0, 1, z },
			{ 0, 0, 0, 1 },
		};
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicVector3<T>::ToInvMatrix() const
	{
		return BasicMatrix<T>{
			{ 1, 0, 0, -x },
			{ 0, 1, 0, -y },
			{ 0, 0, 1, -z },
			{ 0, 0, 0, 1  },
		};
	}

#pragma endregion vector3_implemention



#pragma region vector4_implemention

	template <typename T> force_inline BasicVector4<T>& BasicVector4<T>::operator = (const BasicVector4<T>& inValue) noexcept { x = inValue.x; y = inValue.y; z = inValue.z; w = inValue.w; return *this; }

	template <typename T> force_inline BasicVector2<T> BasicVector4<T>::XY() const { return BasicVector2<T>(x, y); }
	template <typename T> force_inline BasicVector2<T> BasicVector4<T>::ZW() const { return BasicVector2<T>(z, w); }
	template <typename T> force_inline BasicVector3<T> BasicVector4<T>::XYZ() const { return BasicVector3<T>(x, y, z); }
	template <typename T> force_inline BasicVector3<T> BasicVector4<T>::YZW() const { return BasicVector3<T>(y, z, w); }
	template <typename T> force_inline BasicVector2<T>& BasicVector4<T>::XYRef() const { return *(BasicVector2<T>*)(&x); }
	template <typename T> force_inline BasicVector2<T>& BasicVector4<T>::ZWRef() const { return *(BasicVector2<T>*)(&z); }
	template <typename T> force_inline BasicVector3<T>& BasicVector4<T>::XYZRef() const { return *(BasicVector3<T>*)(&x); }
	template <typename T> force_inline BasicVector3<T>& BasicVector4<T>::YZWRef() const { return *(BasicVector3<T>*)(&y); }

	template <typename T> force_inline bool BasicVector4<T>::operator == (const BasicVector4<T>& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
	template <typename T> force_inline bool BasicVector4<T>::operator != (const BasicVector4<T>& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }

	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator + (const BasicVector4<T>& rhs) const { return BasicVector4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator - (const BasicVector4<T>& rhs) const { return BasicVector4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator * (const BasicVector4<T>& rhs) const { return BasicVector4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator / (const BasicVector4<T>& rhs) const { return BasicVector4<T>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator += (const BasicVector4<T>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator -= (const BasicVector4<T>& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator *= (const BasicVector4<T>& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator /= (const BasicVector4<T>& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }

	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator + (const T& rhs) const { return BasicVector4<T>(x + rhs, y + rhs, z + rhs, w + rhs); }
	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator - (const T& rhs) const { return BasicVector4<T>(x - rhs, y - rhs, z - rhs, w - rhs); }
	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator * (const T& rhs) const { return BasicVector4<T>(x * rhs, y * rhs, z * rhs, w * rhs); }
	template <typename T> force_inline BasicVector4<T> BasicVector4<T>::operator / (const T& rhs) const { return BasicVector4<T>(x / rhs, y / rhs, z / rhs, w / rhs); }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator += (const T& rhs) { x += rhs; y += rhs; z += rhs; w += rhs; return *this; }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator -= (const T& rhs) { x -= rhs; y -= rhs; z -= rhs; w -= rhs; return *this; }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator *= (const T& rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	template <typename T> force_inline const BasicVector4<T>& BasicVector4<T>::operator /= (const T& rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

	template <typename T> force_inline T BasicVector4<T>::operator | (const BasicVector4<T>& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }
	template <typename T> force_inline T BasicVector4<T>::DotProduct(const BasicVector4<T>& rhs) const { return (*this) | rhs; }
	template <typename T> force_inline T BasicVector4<T>::Length() const requires is_floating<T> { return std::sqrtf(x * x + y * y + z * z + w * w); }
	template <typename T> force_inline T BasicVector4<T>::LengthSquared() const requires is_floating<T> { return x * x + y * y + z * z + w * w; }

	template <typename T>
	force_inline BasicVector4<T> BasicVector4<T>::Normalize(const float& tolerance) const requires is_floating<T>
	{
		BasicVector4<T> result = *this;
		const T squareSum = result.x * result.x + result.y * result.y + result.z * result.z + result.w * result.w;
		if (squareSum > tolerance)
		{
			const T scale = Math::InvSqrt(squareSum);
			result.x *= scale;
			result.y *= scale;
			result.z *= scale;
			result.w *= scale;
		}
		return result;
	}

	template <typename T>
	force_inline void BasicVector4<T>::Normalized(const float& tolerance) requires is_floating<T>
	{
		const T squareSum = x * x + y * y + z * z + w * w;
		if (squareSum > tolerance)
		{
			const T scale = Math::InvSqrt(squareSum);
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
		}
	}

	template <typename T>
	force_inline bool BasicVector4<T>::IsNormalized(const float& tolerance) const requires is_floating<T>
	{
		const T squareSum = x * x + y * y + z * z + w * w;
		return (std::abs(static_cast<T>(1) - squareSum) < Number::SMALL_NUMBER);
	}

#pragma endregion vector4_implemention



#pragma region matrix_implemention

	template <typename T>
	force_inline BasicMatrix<T>::BasicMatrix(const T& value) noexcept
	{
		m[0][0] = value;  m[0][1] = 0;      m[0][2] = 0;      m[0][3] = 0;
		m[1][0] = 0;      m[1][1] = value;  m[1][2] = 0;      m[1][3] = 0;
		m[2][0] = 0;      m[2][1] = 0;      m[2][2] = value;  m[2][3] = 0;
		m[3][0] = 0;      m[3][1] = 0;      m[3][2] = 0;      m[3][3] = 1;
	}

	template <typename T>
	force_inline BasicMatrix<T>::BasicMatrix(const BasicVector3<T>& row0, const BasicVector3<T>& row1, const BasicVector3<T>& row2, const BasicVector3<T>& row3)
	{
		m[0][0] = row0.x; m[0][1] = row0.y;  m[0][2] = row0.z;  m[0][3] = 0;
		m[1][0] = row1.x; m[1][1] = row1.y;  m[1][2] = row1.z;  m[1][3] = 0;
		m[2][0] = row2.x; m[2][1] = row2.y;  m[2][2] = row2.z;  m[2][3] = 0;
		m[3][0] = row3.x; m[3][1] = row3.y;  m[3][2] = row3.z;  m[3][3] = 1;
	}

	template <typename T>
	force_inline BasicMatrix<T>::BasicMatrix(const BasicVector4<T>& row0, const BasicVector4<T>& row1, const BasicVector4<T>& row2, const BasicVector4<T>& row3)
	{
		m[0][0] = row0.x; m[0][1] = row0.y;  m[0][2] = row0.z;  m[0][3] = row0.w;
		m[1][0] = row1.x; m[1][1] = row1.y;  m[1][2] = row1.z;  m[1][3] = row1.w;
		m[2][0] = row2.x; m[2][1] = row2.y;  m[2][2] = row2.z;  m[2][3] = row2.w;
		m[3][0] = row3.x; m[3][1] = row3.y;  m[3][2] = row3.z;  m[3][3] = row3.w;
	}

	template <typename T>
	force_inline BasicMatrix<T>::BasicMatrix(const BasicMatrix<T>& value)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			Register8Copy(&value.m[0][0], &m[0][0]);
			Register8Copy(&value.m[2][0], &m[2][0]);
		}
		else
		{
			m[0][0] = value.m[0][0]; m[0][1] = value.m[0][1];  m[0][2] = value.m[0][2];  m[0][3] = value.m[0][3];
			m[1][0] = value.m[1][0]; m[1][1] = value.m[1][1];  m[1][2] = value.m[1][2];  m[1][3] = value.m[1][3];
			m[2][0] = value.m[2][0]; m[2][1] = value.m[2][1];  m[2][2] = value.m[2][2];  m[2][3] = value.m[2][3];
			m[3][0] = value.m[3][0]; m[3][1] = value.m[3][1];  m[3][2] = value.m[3][2];  m[3][3] = value.m[3][3];
		}
	}

	template <typename T>
	force_inline BasicMatrix<T>& BasicMatrix<T>::operator = (const BasicMatrix<T>& value)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			Register8Copy(&value.m[0][0], &m[0][0]);
			Register8Copy(&value.m[2][0], &m[2][0]);
		}
		else
		{
			m[0][0] = value.m[0][0]; m[0][1] = value.m[0][1];  m[0][2] = value.m[0][2];  m[0][3] = value.m[0][3];
			m[1][0] = value.m[1][0]; m[1][1] = value.m[1][1];  m[1][2] = value.m[1][2];  m[1][3] = value.m[1][3];
			m[2][0] = value.m[2][0]; m[2][1] = value.m[2][1];  m[2][2] = value.m[2][2];  m[2][3] = value.m[2][3];
			m[3][0] = value.m[3][0]; m[3][1] = value.m[3][1];  m[3][2] = value.m[3][2];  m[3][3] = value.m[3][3];
		}
		return *this;
	}

	template <typename T>
	force_inline BasicMatrix<T>::BasicMatrix(const BasicMatrix<T>&& value) noexcept
	{
		m[0][0] = value.m[0][0]; m[0][1] = value.m[0][1];  m[0][2] = value.m[0][2];  m[0][3] = value.m[0][3];
		m[1][0] = value.m[1][0]; m[1][1] = value.m[1][1];  m[1][2] = value.m[1][2];  m[1][3] = value.m[1][3];
		m[2][0] = value.m[2][0]; m[2][1] = value.m[2][1];  m[2][2] = value.m[2][2];  m[2][3] = value.m[2][3];
		m[3][0] = value.m[3][0]; m[3][1] = value.m[3][1];  m[3][2] = value.m[3][2];  m[3][3] = value.m[3][3];
	}

	template <typename T>
	force_inline BasicMatrix<T>& BasicMatrix<T>::operator = (const BasicMatrix<T>&& value) noexcept
	{
		m[0][0] = value.m[0][0]; m[0][1] = value.m[0][1];  m[0][2] = value.m[0][2];  m[0][3] = value.m[0][3];
		m[1][0] = value.m[1][0]; m[1][1] = value.m[1][1];  m[1][2] = value.m[1][2];  m[1][3] = value.m[1][3];
		m[2][0] = value.m[2][0]; m[2][1] = value.m[2][1];  m[2][2] = value.m[2][2];  m[2][3] = value.m[2][3];
		m[3][0] = value.m[3][0]; m[3][1] = value.m[3][1];  m[3][2] = value.m[3][2];  m[3][3] = value.m[3][3];
		return *this;
	}

	template <typename T>
	force_inline bool BasicMatrix<T>::operator == (const BasicMatrix<T>& rhs) const
	{
		const T* scope_restrict lhsPtr = &m[0][0];
		const T* scope_restrict rhsPtr = &rhs.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++)
			if (*rhsPtr != *lhsPtr) return false;
		return true;
	}

	template <typename T>
	force_inline bool BasicMatrix<T>::operator != (const BasicMatrix<T>& rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::operator + (const BasicMatrix<T>& rhs) const
	{
		BasicMatrix<T> result;
		const T* scope_restrict lhsPtr = &m[0][0];
		const T* scope_restrict rhsPtr = &rhs.m[0][0];
		T* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++, outPtr++)
			*outPtr = *lhsPtr + *rhsPtr;
		return result;
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::operator - (const BasicMatrix<T>& rhs) const
	{
		BasicMatrix<T> result;
		const T* scope_restrict lhsPtr = &m[0][0];
		const T* scope_restrict rhsPtr = &rhs.m[0][0];
		T* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++, outPtr++)
			*outPtr = *lhsPtr - *rhsPtr;
		return result;
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::operator * (const BasicMatrix<T>& rhs) const requires is_floating<T>
	{
		BasicMatrix<T> result;
		if constexpr (std::is_same_v<T, float>)
			Math::MatrixMultiplyMatrix(this, &rhs, &result);
		else
			force_softbreak; // TODO
		return result;
	}

	template <typename T>
	force_inline const BasicMatrix<T>& BasicMatrix<T>::operator += (const BasicMatrix<T>& rhs)
	{
		T* scope_restrict lhsPtr = &m[0][0];
		const T* scope_restrict rhsPtr = &rhs.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++)
			*lhsPtr += *rhsPtr;
		return *this;
	}

	template <typename T>
	force_inline const BasicMatrix<T>& BasicMatrix<T>::operator -= (const BasicMatrix<T>& rhs)
	{
		T* scope_restrict lhsPtr = &m[0][0];
		const T* scope_restrict rhsPtr = &rhs.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++)
			*lhsPtr -= *rhsPtr;
		return *this;
	}

	template <typename T>
	force_inline const BasicMatrix<T>& BasicMatrix<T>::operator *= (const BasicMatrix<T>& rhs) requires is_floating<T>
	{
		if constexpr (std::is_same_v<T, float>)
			Math::MatrixMultiplyMatrix(this, &rhs, this);
		else
			force_softbreak; // TODO
		
		return *this;
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::Transpose() const
	{
		if constexpr (std::is_same_v<T, float>)
		{
			BasicMatrix<T> result = *this;
			Math::MatrixTranspose(&result);
			return result;
		}
		else
		{
			BasicMatrix<T> result;
			result.m[0][0] = m[0][0]; result.m[0][1] = m[1][0];  result.m[0][2] = m[2][0];  result.m[0][3] = m[3][0];
			result.m[1][0] = m[0][1]; result.m[1][1] = m[1][1];  result.m[1][2] = m[2][1];  result.m[1][3] = m[3][1];
			result.m[2][0] = m[0][2]; result.m[2][1] = m[1][2];  result.m[2][2] = m[2][2];  result.m[2][3] = m[3][2];
			result.m[3][0] = m[0][3]; result.m[3][1] = m[1][3];  result.m[3][2] = m[2][3];  result.m[3][3] = m[3][3];
			return result;
		}
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::Inverse() const requires is_floating<T>
	{
		if constexpr (std::is_same_v<T, float>)
		{
			BasicMatrix<T> result;
			Math::MatrixInverse(this, &result);
			return result;
		}
		else
		{
			force_softbreak; // TODO
		}
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::operator * (const T& rhs) const
	{
		BasicMatrix<T> result;
		const T* scope_restrict lhsPtr = &m[0][0];
		T* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, outPtr++)
			*outPtr = *lhsPtr * rhs;
		return result;
	}

	template <typename T>
	force_inline BasicMatrix<T> BasicMatrix<T>::operator / (const T& rhs) const
	{
		BasicMatrix<T> result;
		const T* scope_restrict lhsPtr = &m[0][0];
		T* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, outPtr++)
			*outPtr = *lhsPtr / rhs;
		return result;
	}

	template <typename T>
	force_inline const BasicMatrix<T>& BasicMatrix<T>::operator *= (const T& rhs)
	{
		T* scope_restrict lhsPtr = &m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++)
			*lhsPtr *= rhs;
		return *this;
	}

	template <typename T>
	force_inline const BasicMatrix<T>& BasicMatrix<T>::operator /= (const T& rhs)
	{
		T* scope_restrict lhsPtr = &m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++)
			*lhsPtr /= rhs;
		return *this;
	}

	template <typename T>
	force_inline BasicVector3<T> BasicMatrix<T>::operator * (const BasicVector3<T>& rhs) const requires is_floating<T>
	{
		if constexpr (std::is_same_v<T, float>)
		{
			Vector4f result(rhs, 1.f);
			Math::MatrixMulitplyVectorH(this, &result, &result);
			return result.XYZ();
		}
		else
		{
			force_softbreak; // TODO
		}
	}

	template <typename T>
	force_inline BasicVector4<T> BasicMatrix<T>::operator * (const BasicVector4<T>& rhs) const requires is_floating<T>
	{
		if constexpr (std::is_same_v<T, float>)
		{
			Vector4f result;
			Math::MatrixMulitplyVector(this, &rhs, &result);
			return result;
		}
		else
		{
			force_softbreak; // TODO
		}
	}

#pragma endregion matrix_implemention
}