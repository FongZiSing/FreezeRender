#ifndef MATH_INL_MATH_IPML
#define MATH_INL_MATH_IPML 1

	template <typename T, typename U>
	inline float Math::FastPow(const T& inA, const U& inB)
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

	force_inline float Math::InvSqrt(const float inValue)
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

		const __m128 oneHalf = _mm_set_ss(0.5f);
		__m128 x0, y0, x1, x2, fOver2;

		y0 = _mm_set_ss(inValue);
		x0 = _mm_rsqrt_ss(y0);	// 1/sqrt estimate (12 bits)
		fOver2 = _mm_mul_ss(y0, oneHalf);

		// 1st Newton-Raphson iteration
		x1 = _mm_mul_ss(x0, x0);
		x1 = _mm_sub_ss(oneHalf, _mm_mul_ss(fOver2, x1));
		x1 = _mm_add_ss(x0, _mm_mul_ss(x0, x1));

		// 2nd Newton-Raphson iteration
		x2 = _mm_mul_ss(x1, x1);
		x2 = _mm_sub_ss(oneHalf, _mm_mul_ss(fOver2, x2));
		x2 = _mm_add_ss(x1, _mm_mul_ss(x1, x2));

		return x2.m128_f32[0];
	}

	inline void Math::SinCos(const float inValue, float* outSineResult, float* outCosineResult)
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

	force_inline void Math::VectorSinCos(const void* scope_restrict inAngles, void* scope_restrict outSineResult, void* scope_restrict outCosineResult)
	{
		const __m128& angles = *static_cast<const __m128*>(inAngles);
		__m128& sineResult = *static_cast<__m128*>(outSineResult);
		__m128& cosineResult = *static_cast<__m128*>(outCosineResult);

		// Map to [-pi, pi]
		// X = a - 2pi * round(a/2pi)
		// Note the round(), not truncate(). In this case round() can round halfway cases using round-to-nearest-even OR round-to-nearest.
		// quotient = round(a/2pi)
		__m128 quotient = VectorMultiply(angles, Number::V_ONE_PI2);
		quotient = _mm_cvtepi32_ps(_mm_cvtps_epi32(quotient)); // round to nearest even is the default rounding mode but that's fine here.
		// X = a - 2pi * quotient
		__m128 x = VectorSubtract(angles, VectorMultiply(Number::V_PI2, quotient));

		// Map in [-pi/2,pi/2]
		__m128 sign = VectorAnd(x, Number::V_SIGNBIT);
		__m128 siPI = VectorOr(Number::V_PI, sign);  // pi when x >= 0, -pi when x < 0
		__m128 absX = VectorAbs(x);
		__m128 rflX = VectorSubtract(siPI, x);
		__m128 mask = VectorGT(absX, Number::V_PI_2);
		x = VectorSelect(mask, rflX, x);
		sign = VectorSelect(mask, Number::V_NEGONE, Number::V_ONE);

		const __m128 squaredX = VectorMultiply(x, x);

		// 11-degree minimax approximation
		// *outSineResult = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
		const __m128 sinCoeff0 = MakeVector(1.0f, -0.16666667f, 0.0083333310f, -0.00019840874f);
		const __m128 sinCoeff1 = MakeVector(2.7525562e-06f, -2.3889859e-08f, /*unused*/ 0.f, /*unused*/ 0.f);

		__m128 s;
		s = VectorReplicate(sinCoeff1, 1);
		s = VectorMultiplyAdd(squaredX, s, VectorReplicate(sinCoeff1, 0));
		s = VectorMultiplyAdd(squaredX, s, VectorReplicate(sinCoeff0, 3));
		s = VectorMultiplyAdd(squaredX, s, VectorReplicate(sinCoeff0, 2));
		s = VectorMultiplyAdd(squaredX, s, VectorReplicate(sinCoeff0, 1));
		s = VectorMultiplyAdd(squaredX, s, VectorReplicate(sinCoeff0, 0));
		sineResult = VectorMultiply(s, x);

		// 10-degree minimax approximation
		// *outCosineResult = sign * (((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f);
		const __m128 cosCoeff0 = MakeVector(1.0f, -0.5f, 0.041666638f, -0.0013888378f);
		const __m128 cosCoeff1 = MakeVector(2.4760495e-05f, -2.6051615e-07f, /*unused*/ 0.f, /*unused*/ 0.f);

		__m128 c;
		c = VectorReplicate(cosCoeff1, 1);
		c = VectorMultiplyAdd(squaredX, c, VectorReplicate(cosCoeff1, 0));
		c = VectorMultiplyAdd(squaredX, c, VectorReplicate(cosCoeff0, 3));
		c = VectorMultiplyAdd(squaredX, c, VectorReplicate(cosCoeff0, 2));
		c = VectorMultiplyAdd(squaredX, c, VectorReplicate(cosCoeff0, 1));
		c = VectorMultiplyAdd(squaredX, c, VectorReplicate(cosCoeff0, 0));
		cosineResult = VectorMultiply(c, sign);
	}

	force_inline __m128 Math::Matrix2x2Multiply(const __m128& vec1, const __m128& vec2)
	{
		return VectorAdd(
			VectorMultiply(vec1, VectorSwizzle(vec2, 0, 3, 0, 3)),
			VectorMultiply(VectorSwizzle(vec1, 1, 0, 3, 2), VectorSwizzle(vec2, 2, 1, 2, 1))
		);
	};

	force_inline __m128 Math::Matrix2x2AdjMultiply(const __m128& vec1, const __m128& vec2)
	{
		return VectorSubtract(
			VectorMultiply(VectorSwizzle(vec1, 3, 3, 0, 0), vec2),
			VectorMultiply(VectorSwizzle(vec1, 1, 1, 2, 2), VectorSwizzle(vec2, 2, 3, 0, 1))
		);
	};

	force_inline __m128 Math::Matrix2x2MultiplyAdj(const __m128& vec1, const __m128& vec2)
	{
		return VectorSubtract(
			VectorMultiply(vec1, VectorSwizzle(vec2, 3, 0, 3, 0)),
			VectorMultiply(VectorSwizzle(vec1, 1, 0, 3, 2), VectorSwizzle(vec2, 2, 1, 2, 1))
		);
	};

	force_inline void Math::MatrixTranspose(void* InOutMatrix)
	{
		__m128* M = static_cast<__m128*>(InOutMatrix);
		_MM_TRANSPOSE4_PS(M[0], M[1], M[2], M[3]);
	}

	force_inline void Math::MatrixInverse(const void* inMatrix, void* outMatrix)
	{
		// use block matrix method
		// A is a matrix, then
		//     i(A)    (or iA means)         inverse of A,
		//     a#      (or A_ in code)       means adjugate of A,
		//     |A|     ( or DetA in code)    is determinant,
		//     TR(A)                         is trace
		const __m128* scope_restrict m = static_cast<const __m128*>(inMatrix);

		// sub matrices
		const __m128 a = VectorShuffle(m[0], m[1], 0, 1, 0, 1);
		const __m128 b = VectorShuffle(m[0], m[1], 2, 3, 2, 3);
		const __m128 c = VectorShuffle(m[2], m[3], 0, 1, 0, 1);
		const __m128 d = VectorShuffle(m[2], m[3], 2, 3, 2, 3);

		// determinant as (|A| |B| |C| |D|)
		const __m128 detSub = VectorSubtract(
			VectorMultiply(VectorShuffle(m[0], m[2], 0, 2, 0, 2), VectorShuffle(m[1], m[3], 1, 3, 1, 3)),
			VectorMultiply(VectorShuffle(m[0], m[2], 1, 3, 1, 3), VectorShuffle(m[1], m[3], 0, 2, 0, 2))
		);

		const __m128 detA = VectorSwizzle(detSub, 0, 0, 0, 0);
		const __m128 detB = VectorSwizzle(detSub, 1, 1, 1, 1);
		const __m128 detC = VectorSwizzle(detSub, 2, 2, 2, 2);
		const __m128 detD = VectorSwizzle(detSub, 3, 3, 3, 3);

		// let iM = 1/|M| * | X  Y |
		//                  | Z  W |

		// D#C
		const __m128 d_c = Matrix2x2AdjMultiply(d, c);
		// A#B
		const __m128 a_b = Matrix2x2AdjMultiply(a, b);
		// X# = |D|A - b(D#C)
		__m128 x_ = VectorSubtract(VectorMultiply(detD, a), Matrix2x2Multiply(b, d_c));
		// W# = |A|D - C(A#B)
		__m128 w_ = VectorSubtract(VectorMultiply(detA, d), Matrix2x2Multiply(c, a_b));

		// |M| = |A|*|D| + ... (continue later)
		__m128 detM = VectorMultiply(detA, detD);

		// Y# = |B|C - D(A#B)#
		__m128 y_ = VectorSubtract(VectorMultiply(detB, c), Matrix2x2MultiplyAdj(d, a_b));
		// Z# = |C|B - A(D#C)#
		__m128 z_ = VectorSubtract(VectorMultiply(detC, b), Matrix2x2MultiplyAdj(a, d_c));

		// |M| = |A|*|D| + |B|*|C| ... (continue later)
		detM = VectorAdd(detM, VectorMultiply(detB, detC));

		// tr((A#B)(D#C))
		__m128 tr = VectorMultiply(a_b, VectorSwizzle(d_c, 0, 2, 1, 3));
		tr = VectorAdd(tr, VectorSwizzle(tr, 2, 3, 0, 1));
		tr = VectorAdd(tr, VectorSwizzle(tr, 1, 2, 3, 0));
		// |M| = |A|*|D| + |B|*|C| - TR((A#B)(D#C)
		detM = VectorSubtract(detM, tr);


		if (detM.m128_f32[0] != 0.0f)
		{
			const __m128 adjSignMask = MakeVector(1.f, -1.f, -1.f, 1.f);
			// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
			const __m128 invDetM = VectorDivide(adjSignMask, detM);

			x_ = VectorMultiply(x_, invDetM);
			y_ = VectorMultiply(y_, invDetM);
			z_ = VectorMultiply(z_, invDetM);
			w_ = VectorMultiply(w_, invDetM);

			__m128* scope_restrict result = static_cast<__m128*>(outMatrix);

			// apply adjugate and store, here we combine adjugate shuffle and store shuffle
			result[0] = VectorShuffle(x_, y_, 3, 1, 3, 1);
			result[1] = VectorShuffle(x_, y_, 2, 0, 2, 0);
			result[2] = VectorShuffle(z_, w_, 3, 1, 3, 1);
			result[3] = VectorShuffle(z_, w_, 2, 0, 2, 0);
		}
	}

	force_inline void Math::MatrixMultiplyMatrix(const void* inMatrix1, const void* inMatrix2, void* outMatrix)
	{
		const __m128* m1 = static_cast<const __m128*>(inMatrix1);
		const __m128* m2 = static_cast<const __m128*>(inMatrix2);
		__m128* result = (__m128*)outMatrix;
		__m128 temp, r0, r1, r2, r3;

		// First row of result (inMatrix1[0] * inMatrix2).
		temp = VectorMultiply(VectorReplicate(m1[0], 0), m2[0]);
		temp = VectorMultiplyAdd(VectorReplicate(m1[0], 1), m2[1], temp);
		temp = VectorMultiplyAdd(VectorReplicate(m1[0], 2), m2[2], temp);
		r0 = VectorMultiplyAdd(VectorReplicate(m1[0], 3), m2[3], temp);

		// Second row of result (inMatrix1[1] * inMatrix2).
		temp = VectorMultiply(VectorReplicate(m1[1], 0), m2[0]);
		temp = VectorMultiplyAdd(VectorReplicate(m1[1], 1), m2[1], temp);
		temp = VectorMultiplyAdd(VectorReplicate(m1[1], 2), m2[2], temp);
		r1 = VectorMultiplyAdd(VectorReplicate(m1[1], 3), m2[3], temp);

		// Third row of result (inMatrix1[2] * inMatrix2).
		temp = VectorMultiply(VectorReplicate(m1[2], 0), m2[0]);
		temp = VectorMultiplyAdd(VectorReplicate(m1[2], 1), m2[1], temp);
		temp = VectorMultiplyAdd(VectorReplicate(m1[2], 2), m2[2], temp);
		r2 = VectorMultiplyAdd(VectorReplicate(m1[2], 3), m2[3], temp);

		// Fourth row of result (inMatrix1[3] * inMatrix2).
		temp = VectorMultiply(VectorReplicate(m1[3], 0), m2[0]);
		temp = VectorMultiplyAdd(VectorReplicate(m1[3], 1), m2[1], temp);
		temp = VectorMultiplyAdd(VectorReplicate(m1[3], 2), m2[2], temp);
		r3 = VectorMultiplyAdd(VectorReplicate(m1[3], 3), m2[3], temp);

		// Store result.
		result[0] = r0;
		result[1] = r1;
		result[2] = r2;
		result[3] = r3;
	}

	force_inline void Math::VectorMulitplyMatrix(const void* inVec, const void* inMatrix, void* outVec)
	{
		const __m128& v = *static_cast<const __m128*>(inVec);
		const __m128* scope_restrict m = static_cast<const __m128*>(inMatrix);
		__m128 tempX, tempY, tempZ, tempW;

		// Splat x,y,z and w
		tempX = VectorReplicate(v, 0);
		tempY = VectorReplicate(v, 1);
		tempZ = VectorReplicate(v, 2);
		tempW = VectorReplicate(v, 3);

		// Mul by the matrix
		tempX = VectorMultiply(tempX, m[0]);
		tempY = VectorMultiply(tempY, m[1]);
		tempZ = VectorMultiply(tempZ, m[2]);
		tempW = VectorMultiply(tempW, m[3]);

		// Add them all together
		tempX = VectorAdd(tempX, tempY);
		tempZ = VectorAdd(tempZ, tempW);
		tempX = VectorAdd(tempX, tempZ);

		// Store result.
		VectorStoreAligned(tempX, outVec);
	}

	force_inline void Math::MatrixMulitplyVector(const void* inMatrix, const void* inVec, void* outVec)
	{
		const __m128& v = *static_cast<const __m128*>(inVec);
		const __m128* scope_restrict m = static_cast<const __m128*>(inMatrix);
		__m128 tempX, tempY, tempZ, tempW;

		// Mul by the matrix
		tempX = VectorMultiply(m[0], v);
		tempY = VectorMultiply(m[1], v);
		tempZ = VectorMultiply(m[2], v);
		tempW = VectorMultiply(m[3], v);

		// Add them all together
		_MM_TRANSPOSE4_PS(tempX, tempY, tempZ, tempW);
		tempX = VectorAdd(tempX, tempY);
		tempZ = VectorAdd(tempZ, tempW);
		tempX = VectorAdd(tempX, tempZ);

		// Store result.
		VectorStoreAligned(tempX, outVec);
	}

	force_inline void Math::MatrixMulitplyVectorH(const void* inMatrix, const void* inVec, void* outVec)
	{
		const __m128& v = *static_cast<const __m128*>(inVec);
		const __m128* scope_restrict m = static_cast<const __m128*>(inMatrix);
		__m128 tempX, tempY, tempZ, tempW;

		// Mul by the matrix
		tempX = VectorMultiply(m[0], v);
		tempY = VectorMultiply(m[1], v);
		tempZ = VectorMultiply(m[2], v);
		tempW = VectorMultiply(m[3], v);

		// Add them all together
		_MM_TRANSPOSE4_PS(tempX, tempY, tempZ, tempW);
		tempX = VectorAdd(tempX, tempY);
		tempZ = VectorAdd(tempZ, tempW);
		tempX = VectorAdd(tempX, tempZ);

		// perspective division
		tempZ = VectorReplicate(tempX, 3);
		tempX = VectorDivide(tempX, tempZ);

		// Store result.
		VectorStoreAligned(tempX, outVec);
	}

	force_inline void Math::NormalizeRotator(const void* inVec, void* outVec)
	{
		const float* scope_restrict inR = (const float*)inVec;
		float* scope_restrict outR = (float*)outVec;
		__m128 temp = MakeVector(inR[0], inR[1], inR[2], 0);

		// shift in the range [-360,360]
		__m128 v0 = VectorMod(temp, Number::V_360F);
		__m128 v1 = VectorAdd(v0, Number::V_360F);
		__m128 v2 = VectorSelect(VectorGE(v0, Number::V_ZERO), v0, v1);

		// shift to [-180,180]
		__m128 v3 = VectorSubtract(v2, Number::V_360F);
		__m128 v4 = VectorSelect(VectorGT(v2, Number::V_180F), v3, v2);

		outR[0] = v4.m128_f32[0];
		outR[1] = v4.m128_f32[1];
		outR[2] = v4.m128_f32[2];
	}

#endif // !MATH_INL_MATH_IPML