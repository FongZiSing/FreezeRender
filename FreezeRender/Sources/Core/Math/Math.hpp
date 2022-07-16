//
// Math/Math.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Common Math Library.
//
#pragma once

#include <Common.hpp>
#include <cmath>
#include "SIMD.hpp"



namespace Pluto
{
	/**
	 * @brief Common Math Library.
	 */
	namespace Math
	{
		/**
		 * @brief Clamps `inVal` to be between `inMin` and `inMax`, inclusive.
		 * @return        max( inMin, min( inVal, inMax ) )
		 */
		template <typename T>
		force_inline constexpr std::remove_reference_t<T> Clamp(const T& inMin, const T& inVal, const T& inMax)
		{
			return (inVal < inMin) ? inMin : (inVal < inMax ? inVal : inMax);
		}

		template <typename T>
		force_inline constexpr std::remove_reference_t<T> Clamp(T&& inMin, const T& inVal, T&& inMax)
		{
			return (inVal < inMin) ? inMin : (inVal < inMax ? inVal : inMax);
		}


		/**
		 * @brief Barycentric Interpolation.
		 */
		template <typename T>
		force_inline constexpr std::remove_reference_t<T> Interpolate(const float& alpha, const float& beta, const float& gamma, const T& attribute1, const T& attribute2, const T& attribute3)
		{
			return attribute1 * alpha + attribute2 * beta + attribute3 * gamma;
		}

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

		template <typename T>
		force_inline constexpr std::remove_reference_t<T> Lerp(const float& alpha, T&& attribute1, T&& attribute2)
		{
			return attribute1 + (attribute2 - attribute1) * alpha;
		}

		force_inline void VectorLerp(const float& alpha, const void* inVec1, const void* inVec2, void* outVec)
		{
			const R128& v1 = *static_cast<const R128*>(inVec1);
			const R128& v2 = *static_cast<const R128*>(inVec2);
			R128 temp = RegisterSubtract(v2, v1);
			R128 coeff = MakeRegister(alpha);

			temp = RegisterMultiplyAdd(coeff, temp, v1);
			RegisterStoreAligned(temp, outVec);
		}



		/**
		 * @brief Convert inDegrees to radians.
		 */
		force_inline constexpr float Degrees2Radians(const float& inDegrees)
		{
			return Number::DEG_TO_RAD * inDegrees;
		}

		force_inline void Degrees2Radians(const void* inDegrees, void* outRadians)
		{
			const R128& v1 = *static_cast<const R128*>(inDegrees);
			R128& v2 = *static_cast<R128*>(outRadians);
			v2 = RegisterMultiply(v1, Number::R_DEG_TO_RAD);
		}



		/**
		 * @brief Convert inRadians to degrees.
		 */
		force_inline constexpr float Radians2Degrees(const float& inRadians)
		{
			return Number::RAD_TO_DEG * inRadians;
		}

		force_inline void Radians2Degrees(const void* inRadians, void* outDegrees)
		{
			const R128& v1 = *static_cast<const R128*>(inRadians);
			R128& v2 = *static_cast<R128*>(outDegrees);
			v2 = RegisterMultiply(v1, Number::R_RAD_TO_DEG);
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
		inline float FastPow(const T& inA, const U& inB)
		{
			double a = static_cast<double>(inA);
			double b = static_cast<double>(inB);

			if (a == 0.0) return 0.f;
			if (b == 0.0) return 1.f;

			// calculate approximation with fraction of the exponent
			int e = static_cast<int>(b);
			union
			{
				double d;
				int x[2];
			} u = { a };

			if constexpr (std::endian::native == std::endian::little)
			{
				u.x[1] = static_cast<int>((b - e) * (u.x[1] - 1072632447) + 1072632447);
				u.x[0] = 0;
			}
			else
			{
				u.x[0] = static_cast<int>((b - e) * (u.x[0] - 1072632447) + 1072632447);
				u.x[1] = 0;
			}

			// exponentiation by squaring with the exponent's integer part
			// double R = U.D makes everything much slower, not sure why
			double r = 1.0;
			while (e)
			{
				if (e & 1) { r *= a; }
				a *= a;
				e >>= 1;
			}

			return static_cast<float>(r * u.d);
		}



		/**
		 * @brief Fast Inverse Square Root.
		 * @return        ( 1 / sqrt(inValue) )
		 */
		force_inline float InvSqrt(const float inValue)
		{
			// Performs two passes of Newton-Raphson iteration on the hardware estimate
			//    v^-0.5 = x
			// => x^2 = v^-1
			// => 1/(x^2) = v
			// => F(x) = x^-2 - v
			//    F'(x) = -2x^-3

			//    x1 = x0 - F(x0)/F'(x0)
			// => x1 = x0 + 0.5 * (x0^-2 - vec) * x0^3
			// => x1 = x0 + 0.5 * (x0 - vec * x0^3)
			// => x1 = x0 + x0 * (0.5 - 0.5 * vec * x0^2)
			//
			// This final form has one more operation than the legacy factorization (x1 = 0.5*x0*(3-(Y*x0)*x0)
			// but retains better accuracy (namely InvSqrt(1) = 1 exactly).

			R128 x0, y0, x1, x2, fOver2;

			y0 = _mm_set_ss(inValue);
			x0 = _mm_rsqrt_ss(y0);	// 1/sqrt estimate (12 bits)
			fOver2 = _mm_mul_ss(y0, Number::R_HALF);

			// 1st Newton-Raphson iteration
			x1 = _mm_mul_ss(x0, x0);
			x1 = _mm_sub_ss(Number::R_HALF, _mm_mul_ss(fOver2, x1));
			x1 = _mm_add_ss(x0, _mm_mul_ss(x0, x1));

			// 2nd Newton-Raphson iteration
			x2 = _mm_mul_ss(x1, x1);
			x2 = _mm_sub_ss(Number::R_HALF, _mm_mul_ss(fOver2, x2));
			x2 = _mm_add_ss(x1, _mm_mul_ss(x1, x2));

			return x2.m128_f32[0];
		}



		/**
		 * @brief Fast Inverse Square Root.
		 * @return        ( 1 / sqrt(inValue) )
		 */
		force_inline double InvSqrt(const double inValue)
		{
			// Performs two passes of Newton-Raphson iteration on the hardware estimate
			//    v^-0.5 = x
			// => x^2 = v^-1
			// => 1/(x^2) = v
			// => F(x) = x^-2 - v
			//    F'(x) = -2x^-3

			//    x1 = x0 - F(x0)/F'(x0)
			// => x1 = x0 + 0.5 * (x0^-2 - vec) * x0^3
			// => x1 = x0 + 0.5 * (x0 - vec * x0^3)
			// => x1 = x0 + x0 * (0.5 - 0.5 * vec * x0^2)
			//
			// This final form has one more operation than the legacy factorization (x1 = 0.5*x0*(3-(Y*x0)*x0)
			// but retains better accuracy (namely InvSqrt(1) = 1 exactly).

			R128d x0, y0, x1, x2, fOver2;

			y0 = _mm_set_sd(inValue);
			x0 = _mm_sqrt_sd(Number::R_ONEd, y0);
			x0 = _mm_div_sd(Number::R_ONEd, x0);	// 1/sqrt estimate (12 bits)
			fOver2 = _mm_mul_sd(y0, Number::R_HALFd);

			// 1st Newton-Raphson iteration
			x1 = _mm_mul_sd(x0, x0);
			x1 = _mm_sub_sd(Number::R_HALFd, _mm_mul_sd(fOver2, x1));
			x1 = _mm_add_sd(x0, _mm_mul_sd(x0, x1));

			// 2nd Newton-Raphson iteration
			x2 = _mm_mul_sd(x1, x1);
			x2 = _mm_sub_sd(Number::R_HALFd, _mm_mul_sd(fOver2, x2));
			x2 = _mm_add_sd(x1, _mm_mul_sd(x1, x2));

			return x2.m128d_f64[0];
		}



		/**
		 * @brief Computes the sine and cosine of a number. the same as `XMScalarSinCos` function in <DirectXMath.h>
		 * @see https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmscalarsincos
		 */
		inline void SinCos(const float inValue, float* outSineResult, float* outCosineResult)
		{
			// Map inValue to y in [-pi,pi], x = 2 * pi * quotient + remainder.
			float quotient = (Number::ONE_PI * 0.5f) * inValue;
			if (inValue >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = inValue - (2.0f * Number::PI) * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(inValue).
			float sign;
			if (y > Number::PI_2)
			{
				y = Number::PI - y;
				sign = -1.0f;
			}
			else if (y < -Number::PI_2)
			{
				y = -Number::PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*outSineResult = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float P = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*outCosineResult = sign * P;
		}



		/**
		* @brief Computes the sine and cosine of each element of a vector.
		* @see funciton `SinCos(const float inValue, float* outSineResult, float* outCosineResult)`
		*/
		force_inline void VectorSinCos(const void* scope_restrict inAngles, void* scope_restrict outSineResult, void* scope_restrict outCosineResult)
		{
			const R128& angles = *static_cast<const R128*>(inAngles);
			R128& sineResult = *static_cast<R128*>(outSineResult);
			R128& cosineResult = *static_cast<R128*>(outCosineResult);

			// Map to [-pi, pi]
			// X = a - 2pi * round(a/2pi)
			// Note the round(), not truncate(). In this case round() can round halfway cases using round-to-nearest-even OR round-to-nearest.
			// quotient = round(a/2pi)
			R128 quotient = RegisterMultiply(angles, Number::R_ONE_PI2);
			quotient = _mm_cvtepi32_ps(_mm_cvtps_epi32(quotient)); // round to nearest even is the default rounding mode but that's fine here.
			// X = a - 2pi * quotient
			R128 x = RegisterSubtract(angles, RegisterMultiply(Number::R_PI2, quotient));

			// Map in [-pi/2,pi/2]
			R128 sign = RegisterAnd(x, Number::R_SIGNBIT);
			R128 siPI = RegisterOr(Number::R_PI, sign);  // pi when x >= 0, -pi when x < 0
			R128 absX = RegisterAbs(x);
			R128 rflX = RegisterSubtract(siPI, x);
			R128 mask = RegisterGT(absX, Number::R_PI_2);
			x = RegisterSelect(mask, rflX, x);
			sign = RegisterSelect(mask, Number::R_NEGONE, Number::R_ONE);

			const R128 squaredX = RegisterMultiply(x, x);

			// 11-degree minimax approximation
			// *outSineResult = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
			const R128 sinCoeff0 = MakeRegister(1.0f, -0.16666667f, 0.0083333310f, -0.00019840874f);
			const R128 sinCoeff1 = MakeRegister(2.7525562e-06f, -2.3889859e-08f, /*unused*/ 0.f, /*unused*/ 0.f);

			R128 s;
			s = RegisterReplicate(sinCoeff1, 1);
			s = RegisterMultiplyAdd(squaredX, s, RegisterReplicate(sinCoeff1, 0));
			s = RegisterMultiplyAdd(squaredX, s, RegisterReplicate(sinCoeff0, 3));
			s = RegisterMultiplyAdd(squaredX, s, RegisterReplicate(sinCoeff0, 2));
			s = RegisterMultiplyAdd(squaredX, s, RegisterReplicate(sinCoeff0, 1));
			s = RegisterMultiplyAdd(squaredX, s, RegisterReplicate(sinCoeff0, 0));
			sineResult = RegisterMultiply(s, x);

			// 10-degree minimax approximation
			// *outCosineResult = sign * (((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f);
			const R128 cosCoeff0 = MakeRegister(1.0f, -0.5f, 0.041666638f, -0.0013888378f);
			const R128 cosCoeff1 = MakeRegister(2.4760495e-05f, -2.6051615e-07f, /*unused*/ 0.f, /*unused*/ 0.f);

			R128 c;
			c = RegisterReplicate(cosCoeff1, 1);
			c = RegisterMultiplyAdd(squaredX, c, RegisterReplicate(cosCoeff1, 0));
			c = RegisterMultiplyAdd(squaredX, c, RegisterReplicate(cosCoeff0, 3));
			c = RegisterMultiplyAdd(squaredX, c, RegisterReplicate(cosCoeff0, 2));
			c = RegisterMultiplyAdd(squaredX, c, RegisterReplicate(cosCoeff0, 1));
			c = RegisterMultiplyAdd(squaredX, c, RegisterReplicate(cosCoeff0, 0));
			cosineResult = RegisterMultiply(c, sign);
		}



		/**
		 * @brief 2x2 row major Matrix multiply.
		 * @return        ( a * b )
		 */
		force_inline R128 Matrix2x2Multiply(const R128& vec1, const R128& vec2)
		{
			return RegisterAdd(
				RegisterMultiply(vec1, RegisterSwizzle(vec2, 0, 3, 0, 3)),
				RegisterMultiply(RegisterSwizzle(vec1, 1, 0, 3, 2), RegisterSwizzle(vec2, 2, 1, 2, 1))
			);
		};



		/**
		 * @brief 2x2 row major Matrix adjugate multiply.
		 * @return        ( (a#) * b )
		 */
		force_inline R128 Matrix2x2AdjMultiply(const R128& vec1, const R128& vec2)
		{
			return RegisterSubtract(
				RegisterMultiply(RegisterSwizzle(vec1, 3, 3, 0, 0), vec2),
				RegisterMultiply(RegisterSwizzle(vec1, 1, 1, 2, 2), RegisterSwizzle(vec2, 2, 3, 0, 1))
			);
		};



		/**
		 * @brief 2x2 row major Matrix multiply adjugate.
		 * @return        ( a * (b#) )
		 */
		force_inline R128 Matrix2x2MultiplyAdj(const R128& vec1, const R128& vec2)
		{
			return RegisterSubtract(
				RegisterMultiply(vec1, RegisterSwizzle(vec2, 3, 0, 3, 0)),
				RegisterMultiply(RegisterSwizzle(vec1, 1, 0, 3, 2), RegisterSwizzle(vec2, 2, 1, 2, 1))
			);
		};



		/**
		 * @brief Transpose a 4x4 matrix.
		 */
		force_inline void Matrix44fTranspose(void* InOutMatrix)
		{
			R128* m = static_cast<R128*>(InOutMatrix);
			_MM_TRANSPOSE4_PS(m[0], m[1], m[2], m[3]);
		}

		/**
		 * @brief Transpose a 4x4 matrix.
		 */
		force_inline void Matrix44dTranspose(void* InOutMatrix)
		{
			R256d* m = static_cast<R256d*>(InOutMatrix);
			R256d temp0, temp1, temp2, temp3;
	
			temp0 = _mm256_shuffle_pd(m[0], m[1], 15);
			temp1 = _mm256_shuffle_pd(m[0], m[1], 0);
			temp2 = _mm256_shuffle_pd(m[2], m[3], 15);
			temp3 = _mm256_shuffle_pd(m[2], m[3], 0);
	
			m[0] = _mm256_permute2f128_pd(temp1, temp3, 32);
			m[1] = _mm256_permute2f128_pd(temp0, temp2, 32);
			m[2] = _mm256_permute2f128_pd(temp1, temp3, 49);
			m[3] = _mm256_permute2f128_pd(temp0, temp2, 49);
		}



		/**
		 * @brief Inverse a 4x4 matrix.
		 */
		force_inline void Matrix44fInverse(const void* inMatrix, void* outMatrix)
		{
			// use block matrix method
			// A is a matrix, then
			//     i(A)    (or iA means)         inverse of A,
			//     a#      (or A_ in code)       means adjugate of A,
			//     |A|     ( or DetA in code)    is determinant,
			//     TR(A)                         is trace
			const R128* scope_restrict m = static_cast<const R128*>(inMatrix);

			// sub matrices
			const R128 a = RegisterShuffle(m[0], m[1], 0, 1, 0, 1);
			const R128 b = RegisterShuffle(m[0], m[1], 2, 3, 2, 3);
			const R128 c = RegisterShuffle(m[2], m[3], 0, 1, 0, 1);
			const R128 d = RegisterShuffle(m[2], m[3], 2, 3, 2, 3);

			// determinant as (|A| |B| |C| |D|)
			const R128 detSub = RegisterSubtract(
				RegisterMultiply(RegisterShuffle(m[0], m[2], 0, 2, 0, 2), RegisterShuffle(m[1], m[3], 1, 3, 1, 3)),
				RegisterMultiply(RegisterShuffle(m[0], m[2], 1, 3, 1, 3), RegisterShuffle(m[1], m[3], 0, 2, 0, 2))
			);

			const R128 detA = RegisterSwizzle(detSub, 0, 0, 0, 0);
			const R128 detB = RegisterSwizzle(detSub, 1, 1, 1, 1);
			const R128 detC = RegisterSwizzle(detSub, 2, 2, 2, 2);
			const R128 detD = RegisterSwizzle(detSub, 3, 3, 3, 3);

			// let iM = 1/|M| * | X  Y |
			//                  | Z  W |

			// D#C
			const R128 d_c = Matrix2x2AdjMultiply(d, c);
			// A#B
			const R128 a_b = Matrix2x2AdjMultiply(a, b);
			// X# = |D|A - b(D#C)
			R128 x_ = RegisterSubtract(RegisterMultiply(detD, a), Matrix2x2Multiply(b, d_c));
			// W# = |A|D - C(A#B)
			R128 w_ = RegisterSubtract(RegisterMultiply(detA, d), Matrix2x2Multiply(c, a_b));

			// |M| = |A|*|D| + ... (continue later)
			R128 detM = RegisterMultiply(detA, detD);

			// Y# = |B|C - D(A#B)#
			R128 y_ = RegisterSubtract(RegisterMultiply(detB, c), Matrix2x2MultiplyAdj(d, a_b));
			// Z# = |C|B - A(D#C)#
			R128 z_ = RegisterSubtract(RegisterMultiply(detC, b), Matrix2x2MultiplyAdj(a, d_c));

			// |M| = |A|*|D| + |B|*|C| ... (continue later)
			detM = RegisterAdd(detM, RegisterMultiply(detB, detC));

			// tr((A#B)(D#C))
			R128 tr = RegisterMultiply(a_b, RegisterSwizzle(d_c, 0, 2, 1, 3));
			tr = RegisterAdd(tr, RegisterSwizzle(tr, 2, 3, 0, 1));
			tr = RegisterAdd(tr, RegisterSwizzle(tr, 1, 2, 3, 0));
			// |M| = |A|*|D| + |B|*|C| - TR((A#B)(D#C)
			detM = RegisterSubtract(detM, tr);


			if (detM.m128_f32[0] != 0.0f)
			{
				const R128 adjSignMask = MakeRegister(1.f, -1.f, -1.f, 1.f);
				// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
				const R128 invDetM = RegisterDivide(adjSignMask, detM);

				x_ = RegisterMultiply(x_, invDetM);
				y_ = RegisterMultiply(y_, invDetM);
				z_ = RegisterMultiply(z_, invDetM);
				w_ = RegisterMultiply(w_, invDetM);

				R128* scope_restrict result = static_cast<R128*>(outMatrix);

				// apply adjugate and store, here we combine adjugate shuffle and store shuffle
				result[0] = RegisterShuffle(x_, y_, 3, 1, 3, 1);
				result[1] = RegisterShuffle(x_, y_, 2, 0, 2, 0);
				result[2] = RegisterShuffle(z_, w_, 3, 1, 3, 1);
				result[3] = RegisterShuffle(z_, w_, 2, 0, 2, 0);
			}
		}



		/**
		 * @brief Inverse a 4x4 matrix.
		 */
		force_inline void Matrix44dInverse(const void* inMatrix, void* outMatrix)
		{
			// TODO
			typedef double Double4x4[4][4];
			const Double4x4& m = *((const Double4x4*)inMatrix);
			Double4x4& result = *((Double4x4*)outMatrix);
			Double4x4 tmp;
			double det[4];

			tmp[0][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
			tmp[0][1] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
			tmp[0][2] = m[1][2] * m[2][3] - m[1][3] * m[2][2];

			tmp[1][0] = m[2][2] * m[3][3] - m[2][3] * m[3][2];
			tmp[1][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
			tmp[1][2] = m[0][2] * m[2][3] - m[0][3] * m[2][2];

			tmp[2][0] = m[1][2] * m[3][3] - m[1][3] * m[3][2];
			tmp[2][1] = m[0][2] * m[3][3] - m[0][3] * m[3][2];
			tmp[2][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

			tmp[3][0] = m[1][2] * m[2][3] - m[1][3] * m[2][2];
			tmp[3][1] = m[0][2] * m[2][3] - m[0][3] * m[2][2];
			tmp[3][2] = m[0][2] * m[1][3] - m[0][3] * m[1][2];

			det[0] = m[1][1] * tmp[0][0] - m[2][1] * tmp[0][1] + m[3][1] * tmp[0][2];
			det[1] = m[0][1] * tmp[1][0] - m[2][1] * tmp[1][1] + m[3][1] * tmp[1][2];
			det[2] = m[0][1] * tmp[2][0] - m[1][1] * tmp[2][1] + m[3][1] * tmp[2][2];
			det[3] = m[0][1] * tmp[3][0] - m[1][1] * tmp[3][1] + m[2][1] * tmp[3][2];

			const double Determinant = m[0][0] * det[0] - m[1][0] * det[1] + m[2][0] * det[2] - m[3][0] * det[3];
			const double	RDet = 1.0 / Determinant;

			result[0][0] = RDet * det[0];
			result[0][1] = -RDet * det[1];
			result[0][2] = RDet * det[2];
			result[0][3] = -RDet * det[3];
			result[1][0] = -RDet * (m[1][0] * tmp[0][0] - m[2][0] * tmp[0][1] + m[3][0] * tmp[0][2]);
			result[1][1] = RDet * (m[0][0] * tmp[1][0] - m[2][0] * tmp[1][1] + m[3][0] * tmp[1][2]);
			result[1][2] = -RDet * (m[0][0] * tmp[2][0] - m[1][0] * tmp[2][1] + m[3][0] * tmp[2][2]);
			result[1][3] = RDet * (m[0][0] * tmp[3][0] - m[1][0] * tmp[3][1] + m[2][0] * tmp[3][2]);
			result[2][0] = RDet * (
				m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
				m[2][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) +
				m[3][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1])
				);
			result[2][1] = -RDet * (
				m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) -
				m[2][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
				m[3][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1])
				);
			result[2][2] = RDet * (
				m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) -
				m[1][0] * (m[0][1] * m[3][3] - m[0][3] * m[3][1]) +
				m[3][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
				);
			result[2][3] = -RDet * (
				m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) -
				m[1][0] * (m[0][1] * m[2][3] - m[0][3] * m[2][1]) +
				m[2][0] * (m[0][1] * m[1][3] - m[0][3] * m[1][1])
				);
			result[3][0] = -RDet * (
				m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
				m[2][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) +
				m[3][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
				);
			result[3][1] = RDet * (
				m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) -
				m[2][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
				m[3][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
				);
			result[3][2] = -RDet * (
				m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) -
				m[1][0] * (m[0][1] * m[3][2] - m[0][2] * m[3][1]) +
				m[3][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
				);
			result[3][3] = RDet * (
				m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
				m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1]) +
				m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1])
				);
		}



		/**
		 * @brief Multiplies two 4x4 matrices.
		 * @param outMatrix    - inMatrix1 * inMatrix2
		 */
		force_inline void Matrix44fMultiplyMatrix44f(const void* inMatrix1, const void* inMatrix2, void* outMatrix)
		{
			const R128* m1 = static_cast<const R128*>(inMatrix1);
			const R128* m2 = static_cast<const R128*>(inMatrix2);
			R128* result = (R128*)outMatrix;
			R128 temp, r0, r1, r2, r3;

			// First row of result (inMatrix1[0] * inMatrix2).
			temp = RegisterMultiply(RegisterReplicate(m1[0], 0), m2[0]);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[0], 1), m2[1], temp);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[0], 2), m2[2], temp);
			r0 = RegisterMultiplyAdd(RegisterReplicate(m1[0], 3), m2[3], temp);

			// Second row of result (inMatrix1[1] * inMatrix2).
			temp = RegisterMultiply(RegisterReplicate(m1[1], 0), m2[0]);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[1], 1), m2[1], temp);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[1], 2), m2[2], temp);
			r1 = RegisterMultiplyAdd(RegisterReplicate(m1[1], 3), m2[3], temp);

			// Third row of result (inMatrix1[2] * inMatrix2).
			temp = RegisterMultiply(RegisterReplicate(m1[2], 0), m2[0]);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[2], 1), m2[1], temp);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[2], 2), m2[2], temp);
			r2 = RegisterMultiplyAdd(RegisterReplicate(m1[2], 3), m2[3], temp);

			// Fourth row of result (inMatrix1[3] * inMatrix2).
			temp = RegisterMultiply(RegisterReplicate(m1[3], 0), m2[0]);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[3], 1), m2[1], temp);
			temp = RegisterMultiplyAdd(RegisterReplicate(m1[3], 2), m2[2], temp);
			r3 = RegisterMultiplyAdd(RegisterReplicate(m1[3], 3), m2[3], temp);

			// Store result.
			result[0] = r0;
			result[1] = r1;
			result[2] = r2;
			result[3] = r3;
		}



		/**
		 * @brief Multiplies two 4x4 matrices.
		 * @param outMatrix    - inMatrix1 * inMatrix2
		 */
		force_inline void Matrix44dMultiplyMatrix44d(const void* inMatrix1, const void* inMatrix2, void* outMatrix)
		{
			const R256d* m1 = static_cast<const R256d*>(inMatrix1);
			const R256d* m2 = static_cast<const R256d*>(inMatrix2);
			R256d* result = (R256d*)outMatrix;
			R256d temp, r0, r1, r2, r3;

			// First row of result (inMatrix1[0] * inMatrix2).
			temp = Register4dMultiply(Register4dReplicate<0>(m1[0]), m2[0]);
			temp = Register4dMultiplyAdd(Register4dReplicate<1>(m1[0]), m2[1], temp);
			temp = Register4dMultiplyAdd(Register4dReplicate<2>(m1[0]), m2[2], temp);
			r0 = Register4dMultiplyAdd(Register4dReplicate<3>(m1[0]), m2[3], temp);

			// Second row of result (inMatrix1[1] * inMatrix2).
			temp = Register4dMultiply(Register4dReplicate<0>(m1[1]), m2[0]);
			temp = Register4dMultiplyAdd(Register4dReplicate<1>(m1[1]), m2[1], temp);
			temp = Register4dMultiplyAdd(Register4dReplicate<2>(m1[1]), m2[2], temp);
			r1 = Register4dMultiplyAdd(Register4dReplicate<3>(m1[1]), m2[3], temp);

			// Third row of result (inMatrix1[2] * inMatrix2).
			temp = Register4dMultiply(Register4dReplicate<0>(m1[2]), m2[0]);
			temp = Register4dMultiplyAdd(Register4dReplicate<1>(m1[2]), m2[1], temp);
			temp = Register4dMultiplyAdd(Register4dReplicate<2>(m1[2]), m2[2], temp);
			r2 = Register4dMultiplyAdd(Register4dReplicate<3>(m1[2]), m2[3], temp);

			// Fourth row of result (inMatrix1[3] * inMatrix2).
			temp = Register4dMultiply(Register4dReplicate<0>(m1[3]), m2[0]);
			temp = Register4dMultiplyAdd(Register4dReplicate<1>(m1[3]), m2[1], temp);
			temp = Register4dMultiplyAdd(Register4dReplicate<2>(m1[3]), m2[2], temp);
			r3 = Register4dMultiplyAdd(Register4dReplicate<3>(m1[3]), m2[3], temp);

			// Store result.
			result[0] = r0;
			result[1] = r1;
			result[2] = r2;
			result[3] = r3;
		}



		/**
		 * @brief Multiplies 1x4 vector and 4x4 matrix.
		 * @param outVec    - inVec * inMatrix
		 */
		force_inline void Vector4fMulitplyMatrix44f(const void* inVec, const void* inMatrix, void* outVec)
		{
			const R128& v = *static_cast<const R128*>(inVec);
			const R128* scope_restrict m = static_cast<const R128*>(inMatrix);
			R128 tempX, tempY, tempZ, tempW;

			// Splat x,y,z and w
			tempX = RegisterReplicate(v, 0);
			tempY = RegisterReplicate(v, 1);
			tempZ = RegisterReplicate(v, 2);
			tempW = RegisterReplicate(v, 3);

			// Mul by the matrix
			tempX = RegisterMultiply(tempX, m[0]);
			tempY = RegisterMultiply(tempY, m[1]);
			tempZ = RegisterMultiply(tempZ, m[2]);
			tempW = RegisterMultiply(tempW, m[3]);

			// Add them all together
			tempX = RegisterAdd(tempX, tempY);
			tempZ = RegisterAdd(tempZ, tempW);
			tempX = RegisterAdd(tempX, tempZ);

			// Store result.
			RegisterStoreAligned(tempX, outVec);
		}



		/**
		 * @brief Multiplies 1x4 vector and 4x4 matrix.
		 * @param outVec    - inVec * inMatrix
		 */
		force_inline void Vector4dMulitplyMatrix44d(const void* inVec, const void* inMatrix, void* outVec)
		{
			const R256d& v = *static_cast<const R256d*>(inVec);
			const R256d* scope_restrict m = static_cast<const R256d*>(inMatrix);
			R256d tempX, tempY, tempZ, tempW;

			// Splat x,y,z and w
			tempX = Register4dReplicate<0>(v);
			tempY = Register4dReplicate<1>(v);
			tempZ = Register4dReplicate<2>(v);
			tempW = Register4dReplicate<3>(v);

			// Mul by the matrix
			tempX = Register4dMultiply(tempX, m[0]);
			tempY = Register4dMultiply(tempY, m[1]);
			tempZ = Register4dMultiply(tempZ, m[2]);
			tempW = Register4dMultiply(tempW, m[3]);

			// Add them all together
			tempX = Register4dAdd(tempX, tempY);
			tempZ = Register4dAdd(tempZ, tempW);
			tempX = Register4dAdd(tempX, tempZ);

			// Store result.
			Register4dStoreAligned(tempX, outVec);
		}



		/**
		 * @brief Multiplies 4x4 matrix and 4x1 vector.
		 * @param outVec    - inMatrix * inVec
		 */
		force_inline void Matrix44fMulitplyVector4f(const void* inMatrix, const void* inVec, void* outVec)
		{
			const R128& v = *static_cast<const R128*>(inVec);
			const R128* scope_restrict m = static_cast<const R128*>(inMatrix);
			R128 tempX, tempY, tempZ, tempW;

			// Mul by the matrix
			tempX = RegisterMultiply(m[0], v);
			tempY = RegisterMultiply(m[1], v);
			tempZ = RegisterMultiply(m[2], v);
			tempW = RegisterMultiply(m[3], v);

			// Add them all together
			_MM_TRANSPOSE4_PS(tempX, tempY, tempZ, tempW);
			tempX = RegisterAdd(tempX, tempY);
			tempZ = RegisterAdd(tempZ, tempW);
			tempX = RegisterAdd(tempX, tempZ);

			// Store result.
			RegisterStoreAligned(tempX, outVec);
		}



		/**
		 * @brief Multiplies 4x4 matrix and 4x1 vector.
		 * @param outVec    - inMatrix * inVec
		 */
		force_inline void Matrix44dMulitplyVector4d(const void* inMatrix, const void* inVec, void* outVec)
		{
			const R256d& v = *static_cast<const R256d*>(inVec);
			const R256d* scope_restrict m = static_cast<const R256d*>(inMatrix);
			R256d tempX, tempY, tempZ, tempW;

			// Mul by the matrix
			tempX = Register4dMultiply(m[0], v);
			tempY = Register4dMultiply(m[1], v);
			tempZ = Register4dMultiply(m[2], v);
			tempW = Register4dMultiply(m[3], v);

			// Add them all together
			R256d temp0, temp1, temp2, temp3;
			temp0 = _mm256_shuffle_pd(tempX, tempY, 15);
			temp1 = _mm256_shuffle_pd(tempX, tempY, 0);
			temp2 = _mm256_shuffle_pd(tempZ, tempW, 15);
			temp3 = _mm256_shuffle_pd(tempZ, tempW, 0);

			tempX = _mm256_permute2f128_pd(temp1, temp3, 32);
			tempY = _mm256_permute2f128_pd(temp0, temp2, 32);
			tempZ = _mm256_permute2f128_pd(temp1, temp3, 49);
			tempW = _mm256_permute2f128_pd(temp0, temp2, 49);

			tempX = Register4dAdd(tempX, tempY);
			tempZ = Register4dAdd(tempZ, tempW);
			tempX = Register4dAdd(tempX, tempZ);

			// Store result.
			Register4dStoreAligned(tempX, outVec);
		}



		/**
		 * @brief Multiplies 4x4 matrix and 4x1 vector, and do perspective division.
		 * @param outVec    - inMatrix * inVec
		 */
		force_inline void Matrix44fMulitplyVector4fH(const void* inMatrix, const void* inVec, void* outVec)
		{
			const R128& v = *static_cast<const R128*>(inVec);
			const R128* scope_restrict m = static_cast<const R128*>(inMatrix);
			R128 tempX, tempY, tempZ, tempW;

			// Mul by the matrix
			tempX = RegisterMultiply(m[0], v);
			tempY = RegisterMultiply(m[1], v);
			tempZ = RegisterMultiply(m[2], v);
			tempW = RegisterMultiply(m[3], v);

			// Add them all together
			_MM_TRANSPOSE4_PS(tempX, tempY, tempZ, tempW);
			tempX = RegisterAdd(tempX, tempY);
			tempZ = RegisterAdd(tempZ, tempW);
			tempX = RegisterAdd(tempX, tempZ);

			// perspective division
			tempZ = RegisterReplicate(tempX, 3);
			tempX = RegisterDivide(tempX, tempZ);

			// Store result.
			RegisterStoreAligned(tempX, outVec);
		}



		/**
		 * @brief Multiplies 4x4 matrix and 4x1 vector, and do perspective division.
		 * @param outVec    - inMatrix * inVec
		 */
		force_inline void Matrix44dMulitplyVector4dH(const void* inMatrix, const void* inVec, void* outVec)
		{
			const R256d& v = *static_cast<const R256d*>(inVec);
			const R256d* scope_restrict m = static_cast<const R256d*>(inMatrix);
			R256d tempX, tempY, tempZ, tempW;

			// Mul by the matrix
			tempX = Register4dMultiply(m[0], v);
			tempY = Register4dMultiply(m[1], v);
			tempZ = Register4dMultiply(m[2], v);
			tempW = Register4dMultiply(m[3], v);

			// Add them all together
			R256d temp0, temp1, temp2, temp3;
			temp0 = _mm256_shuffle_pd(tempX, tempY, 15);
			temp1 = _mm256_shuffle_pd(tempX, tempY, 0);
			temp2 = _mm256_shuffle_pd(tempZ, tempW, 15);
			temp3 = _mm256_shuffle_pd(tempZ, tempW, 0);

			tempX = _mm256_permute2f128_pd(temp1, temp3, 32);
			tempY = _mm256_permute2f128_pd(temp0, temp2, 32);
			tempZ = _mm256_permute2f128_pd(temp1, temp3, 49);
			tempW = _mm256_permute2f128_pd(temp0, temp2, 49);


			tempX = Register4dAdd(tempX, tempY);
			tempZ = Register4dAdd(tempZ, tempW);
			tempX = Register4dAdd(tempX, tempZ);

			// perspective division
			tempZ = Register4dReplicate<3>(tempX);
			tempX = Register4dDivide(tempX, tempZ);

			// Store result.
			Register4dStoreAligned(tempX, outVec);
		}



		/**
		 * @brief Normalize rotator, shift to [-180, 180].
		 */
		force_inline void NormalizeRotator(const void* inVec, void* outVec)
		{
			const float* scope_restrict inR = (const float*)inVec;
			float* scope_restrict outR = (float*)outVec;
			R128 temp = MakeRegister(inR[0], inR[1], inR[2], 0);

			// shift in the range [-360,360]
			R128 v0 = RegisterMod(temp, Number::R_360F);
			R128 v1 = RegisterAdd(v0, Number::R_360F);
			R128 v2 = RegisterSelect(RegisterGE(v0, Number::R_ZERO), v0, v1);

			// shift to [-180,180]
			R128 v3 = RegisterSubtract(v2, Number::R_360F);
			R128 v4 = RegisterSelect(RegisterGT(v2, Number::R_180F), v3, v2);

			outR[0] = v4.m128_f32[0];
			outR[1] = v4.m128_f32[1];
			outR[2] = v4.m128_f32[2];
		}
	}
}