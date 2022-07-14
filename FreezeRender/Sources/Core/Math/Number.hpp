//
// Math/Number.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of useful mathematical constants.
//
#pragma once



namespace Pluto
{
	/**
	 * @brief Definitions of useful mathematical constants.
	 */
	namespace Number
	{
		constexpr const float E                    = static_cast<float>(2.71828182845904523536);   // e
		constexpr const float LOG2E                = static_cast<float>(1.44269504088896340736);   // log2(e)
		constexpr const float LOG10E               = static_cast<float>(0.434294481903251827651);  // log10(e)
		constexpr const float LN2                  = static_cast<float>(0.693147180559945309417);  // ln(2)
		constexpr const float LN10                 = static_cast<float>(2.30258509299404568402);   // ln(10)
		constexpr const float PI                   = static_cast<float>(3.14159265358979323846);   // pi
		constexpr const float PI_2                 = static_cast<float>(1.57079632679489661923);   // pi/2
		constexpr const float PI_4                 = static_cast<float>(0.785398163397448309616);  // pi/4
		constexpr const float ONE_PI               = static_cast<float>(0.318309886183790671538);  // 1/pi
		constexpr const float TWO_PI               = static_cast<float>(0.636619772367581343076);  // 2/pi
		constexpr const float TWO_SQRTPI           = static_cast<float>(1.12837916709551257390);   // 2/sqrt(pi)
		constexpr const float SQRT2                = static_cast<float>(1.41421356237309504880);   // sqrt(2)
		constexpr const float ONE_SQRT2            = static_cast<float>(0.707106781186547524401);  // 1/sqrt(2)
	
		constexpr const float DEG_TO_RAD           = PI / 180.f;
		constexpr const float RAD_TO_DEG           = 180.f / PI;
	
		constexpr const float FLOAT_NON_FRACTIONAL = 8388608.f;

		constexpr const float SMALL_NUMBER         = 1e-6f;
		constexpr const float FLOAT_NAN            = __builtin_nanf("0");
		constexpr const float FLOAT_SIGNAL_NAN     = __builtin_nansf("1");
		constexpr const float FLOAT_INF            = __builtin_huge_valf();
		constexpr const float FLOAT_NEG_INF        = -__builtin_huge_valf();
	}
}