#pragma once

#include <Math/SIMD.hpp>



/**
 * @brief The implemention of Kahan Summation Algorithm.
 * @see https://en.wikipedia.org/wiki/Kahan_summation_algorithm
 */
struct KahanSum final
{
	KahanSum(const float& init) noexcept
		: sum(init)
		, error(0.f)
	{}

	force_inline float& operator += (float& increment) noexcept
	{
		float temp = increment - error;
		float result = sum + temp;
		error = (result - sum) - increment;
		sum = result;
		return sum;
	}

	force_inline float& Value() noexcept
	{
		return sum;
	}

private:
	float sum;
	float error;
};



/**
 * @brief The implemention of Kahan Summation Algorithm (SIMD Version).
 * @see https://en.wikipedia.org/wiki/Kahan_summation_algorithm
 */
struct KahanSum4 final
{
	KahanSum4(const R128& init) noexcept
		: sum(init)
		, error(Number::R_ZERO)
	{}

	force_inline R128& operator += (R128& increment) noexcept
	{
		R128 temp = RegisterSubtract(increment, error);
		R128 result = RegisterAdd(sum, temp);
		error = RegisterSubtract(RegisterSubtract(result, sum), increment);
		sum = result;
		return sum;
	}

	force_inline R128& Value() noexcept
	{
		return sum;
	}

private:
	R128 sum;
	R128 error;
};