#pragma once

#include <Common.hpp>
#include <algorithm>
#include <cmath>
#include <bit>

#include "SIMD.hpp"



/**
 * @brief Common Math Library.
*/
namespace Math
{
	/**
	 * @brief The implemention of Kanhan Sumation Algorithm.
	 * @see https://en.wikipedia.org/wiki/Kahan_summation_algorithm
	 */
	struct VectorKahanSummationAlgorithm
	{
		__m128 error = Number::V_ZERO;

		force_inline void Add(__m128& sum, __m128& increment) noexcept
		{
			__m128 temp = VectorSubtract(increment, error);
			__m128 result = VectorAdd(sum, temp);
			error = VectorSubtract(VectorSubtract(result, sum), increment);
			sum = result;
		}
	};

	/**
	 * @brief <minwindef.h> defined marco `max`, `min`, conflicts with std::max, std::min.
	 */
	template <typename T> force_inline constexpr T Min(const T& inA, const T& inB) { return std::min<T>(inA, inB); }
	template <typename T> force_inline constexpr T Max(const T& inA, const T& inB) { return std::max<T>(inA, inB); }
	template <typename T> force_inline constexpr T Min(std::initializer_list<T> inList) { return std::min<T>(inList); }
	template <typename T> force_inline constexpr T Max(std::initializer_list<T> inList) { return std::max<T>(inList); }

	/**
	 * @brief Clamps `inVal` to be between `inMin` and `inMax`, inclusive.
	 * @return        max( inMin, min( inVal, inMax ) )
	 */
	template <typename T>
	force_inline constexpr std::remove_reference_t<T> Clamp(const T& inMin, const T& inVal, const T& inMax)
	{
		return (inVal < inMin) ? inMin : (inVal < inMax ? inVal : inMax);
	}

	/**
	 * @brief Clamps `inVal` to be between `inMin` and `inMax`, inclusive.
	 * @return        max( inMin, min( inVal, inMax ) )
	 */
	template <typename T>
	force_inline constexpr std::remove_reference_t<T> Clamp(T&& inMin, const T& inVal, T&& inMax)
	{
		return (inVal < inMin) ? inMin : (inVal < inMax ? inVal : inMax);
	}

	/**
	 * @brief Barycentric Interpolation.
	 * @see https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
	 */
	template <typename T>
	force_inline constexpr std::remove_reference_t<T> Interpolate(const float& alpha, const float& beta, const float& gamma, const T& attribute1, const T& attribute2, const T& attribute3)
	{
		return attribute1 * alpha + attribute2 * beta + attribute3 * gamma;
	}

	/**
	 * @brief Barycentric Interpolation.
	 * @see https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
	 */
	template <typename T>
	force_inline constexpr std::remove_reference_t<T> Interpolate(const float& alpha, const float& beta, const float& gamma, T&& attribute1, T&& attribute2, T&& attribute3)
	{
		return attribute1 * alpha + attribute2 * beta + attribute3 * gamma;
	}

	/**
	 * @brief Interpolation.
	 */
	template <typename T>
	force_inline constexpr std::remove_reference_t<T> Lerp(const float& alpha, const T& attribute1, const T& attribute2)
	{
		return attribute1 + (attribute2 - attribute1) * alpha;
	}

	/**
	 * @brief Interpolation.
	 */
	template <typename T>
	force_inline constexpr std::remove_reference_t<T> Lerp(const float& alpha, T&& attribute1, T&& attribute2)
	{
		return attribute1 + (attribute2 - attribute1) * alpha;
	}

	/**
	 * @brief Linear vector interpolation.
	 * @param outVec    - (*inVec1) + alpha * ( (*inVec2) - (*inVec1) )
	 */
	force_inline void VectorLerp(const float& alpha, const void* inVec1, const void* inVec2, void* outVec)
	{
		const __m128& v1 = *static_cast<const __m128*>(inVec1);
		const __m128& v2 = *static_cast<const __m128*>(inVec2);
		__m128 temp = VectorSubtract(v2, v1);
		__m128 coeff = MakeVector(alpha);

		temp = VectorMultiplyAdd(coeff, temp, v1);
		VectorStoreAligned(temp, outVec);
	}

	/**
	 * @brief Convert inDegrees to radians.
	 */
	force_inline constexpr float Degrees2Radians(const float& inDegrees)
	{
		return Number::DEG_TO_RAD * inDegrees;
	}

	/**
	 * @brief Convert 4 degrees to radians.
	 */
	force_inline void Degrees2Radians(const void* inDegrees, void* outRadians)
	{
		const __m128& v1 = *static_cast<const __m128*>(inDegrees);
		__m128& v2 = *static_cast<__m128*>(outRadians);
		v2 = VectorMultiply(v1, Number::V_DEG_TO_RAD);
	}

	/**
	 * @brief Convert inRadians to degrees.
	 */
	force_inline constexpr float Radians2Degrees(const float& inRadians)
	{
		return Number::RAD_TO_DEG * inRadians;
	}

	/**
	 * @brief Convert 4 degrees to radians.
	 */
	force_inline void Radians2Degrees(const void* inRadians, void* outDegrees)
	{
		const __m128& v1 = *static_cast<const __m128*>(inRadians);
		__m128& v2 = *static_cast<__m128*>(outDegrees);
		v2 = VectorMultiply(v1, Number::V_RAD_TO_DEG);
	}


	/**
	 * @brief Determines whether `inX` is between [inMinX, inMaxX) and `inY` is between [inMinY, inMaxY).
	 * @return        ( inX >= inMinX && inX < inMaxX && inY >= inMinY && inY < inMaxY )
	 */
	template <typename T>
	force_inline constexpr bool IsWithin(const T& inMinX, const T& inX, const T& inMaxX, const T& inMinY, const T& inY, const T& inMaxY)
	{
		static_assert(std::is_same_v<std::decay_t<T>, int> || std::is_same_v<std::decay_t<T>, short>, "[FreezeRender] only support int and short!");
		return
			static_cast<unsigned>(inX - inMinX) < static_cast<unsigned>(inMaxX - inMinX) &&
			static_cast<unsigned>(inY - inMinY) < static_cast<unsigned>(inMaxY - inMinY);
	}
	
	/**
	 * @brief At least 3.3 times faster than std::pow().
	 * @see https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
	 */
	template <typename T, typename U>
	inline float FastPow(const T& inA, const U& inB);

	/**
	 * @brief Fast Inverse Square Root.
	 * @return        ( 1 / sqrt(inValue) )
	 */
	force_inline float InvSqrt(const float inValue);

	/**
	 * @brief Computes the sine and cosine of a number. the same as `XMScalarSinCos` function in <DirectXMath.h>
	 * @see https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmscalarsincos
	 */
	inline void SinCos(const float inValue, float* outSineResult, float* outCosineResult);

	/**
	* @brief Computes the sine and cosine of each element of a vector.
	* @see funciton `SinCos(const float inValue, float* outSineResult, float* outCosineResult)`
	*/
	force_inline void VectorSinCos(const void* scope_restrict inAngles, void* scope_restrict outSineResult, void* scope_restrict outCosineResult);

	/**
	 * @brief 2x2 row major Matrix multiply.
	 * @return        ( a * b )
	 */
	force_inline __m128 Matrix2x2Multiply(const __m128& vec1, const __m128& vec2);

	/**
	 * @brief 2x2 row major Matrix adjugate multiply.
	 * @return        ( (a#) * b )
	 */
	force_inline __m128 Matrix2x2AdjMultiply(const __m128& vec1, const __m128& vec2);

	/**
	 * @brief 2x2 row major Matrix multiply adjugate.
	 * @return        ( a * (b#) )
	 */
	force_inline __m128 Matrix2x2MultiplyAdj(const __m128& vec1, const __m128& vec2);

	/**
	 * @brief Transpose a 4x4 matrix.
	 */
	force_inline void MatrixTranspose(void* InOutMatrix);

	/**
	 * @brief Inverse a 4x4 matrix.
	 */
	force_inline void MatrixInverse(const void* inMatrix, void* outMatrix);

	/**
	 * @brief Multiplies two 4x4 matrices.
	 * @param outMatrix    - inMatrix1 * inMatrix2
	 */
	force_inline void MatrixMultiplyMatrix(const void* inMatrix1, const void* inMatrix2, void* outMatrix);

	/**
	 * @brief Multiplies 1x4 vector and 4x4 matrix.
	 * @param outVec    - inVec * inMatrix
	 */
	force_inline void VectorMulitplyMatrix(const void* inVec, const void* inMatrix, void* outVec);

	/**
	 * @brief Multiplies 4x4 matrix and 4x1 vector.
	 * @param outVec    - inMatrix * inVec
	 */
	force_inline void MatrixMulitplyVector(const void* inMatrix, const void* inVec, void* outVec);

	/**
	 * @brief Multiplies 4x4 matrix and 4x1 vector, and do perspective division.
	 * @param outVec    - inMatrix * inVec
	 */
	force_inline void MatrixMulitplyVectorH(const void* inMatrix, const void* inVec, void* outVec);

	/**
	 * @brief Normalize rotator, shift to [-180, 180].
	 */
	force_inline void NormalizeRotator(const void* inVec, void* outVec);
}



/**
 * @brief The Detail implemention of `Math` library.
 */
#include "Math.inl"