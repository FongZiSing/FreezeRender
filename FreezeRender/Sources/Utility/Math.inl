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

		const R128 oneHalf = _mm_set_ss(0.5f);
		R128 x0, y0, x1, x2, fOver2;

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

	force_inline R128 Math::Matrix2x2Multiply(const R128& vec1, const R128& vec2)
	{
		return RegisterAdd(
			RegisterMultiply(vec1, RegisterSwizzle(vec2, 0, 3, 0, 3)),
			RegisterMultiply(RegisterSwizzle(vec1, 1, 0, 3, 2), RegisterSwizzle(vec2, 2, 1, 2, 1))
		);
	};

	force_inline R128 Math::Matrix2x2AdjMultiply(const R128& vec1, const R128& vec2)
	{
		return RegisterSubtract(
			RegisterMultiply(RegisterSwizzle(vec1, 3, 3, 0, 0), vec2),
			RegisterMultiply(RegisterSwizzle(vec1, 1, 1, 2, 2), RegisterSwizzle(vec2, 2, 3, 0, 1))
		);
	};

	force_inline R128 Math::Matrix2x2MultiplyAdj(const R128& vec1, const R128& vec2)
	{
		return RegisterSubtract(
			RegisterMultiply(vec1, RegisterSwizzle(vec2, 3, 0, 3, 0)),
			RegisterMultiply(RegisterSwizzle(vec1, 1, 0, 3, 2), RegisterSwizzle(vec2, 2, 1, 2, 1))
		);
	};

	force_inline void Math::MatrixTranspose(void* InOutMatrix)
	{
		R128* M = static_cast<R128*>(InOutMatrix);
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

	force_inline void Math::MatrixMultiplyMatrix(const void* inMatrix1, const void* inMatrix2, void* outMatrix)
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

	force_inline void Math::VectorMulitplyMatrix(const void* inVec, const void* inMatrix, void* outVec)
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

	force_inline void Math::MatrixMulitplyVector(const void* inMatrix, const void* inVec, void* outVec)
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

	force_inline void Math::MatrixMulitplyVectorH(const void* inMatrix, const void* inVec, void* outVec)
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

	force_inline void Math::NormalizeRotator(const void* inVec, void* outVec)
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

#endif // !MATH_INL_MATH_IPML