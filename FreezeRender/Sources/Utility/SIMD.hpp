#pragma once

#include <Common.hpp>
#include <immintrin.h>
#include "Number.hpp"



namespace Number
{
	force_inline constexpr __m128 MakeVector(const unsigned int& x, const unsigned int& y, const unsigned int& z, const unsigned int& w)
	{
		__m128 result;
		result.m128_u32[0] = x;
		result.m128_u32[1] = y;
		result.m128_u32[2] = z;
		result.m128_u32[3] = w;
		return result;
	}

	force_inline constexpr __m128 MakeVector(const float& x, const float& y, const float& z, const float& w)
	{
		__m128 result;
		result.m128_f32[0] = x;
		result.m128_f32[1] = y;
		result.m128_f32[2] = z;
		result.m128_f32[3] = w;
		return result;
	}

	constexpr const __m128 V_ZERO          = MakeVector( 0.f, 0.f, 0.f, 0.f );
	constexpr const __m128 V_ONE           = MakeVector(  1.f,  1.f,  1.f,  1.f );
	constexpr const __m128 V_NEGONE        = MakeVector( -1.f, -1.f, -1.f, -1.f );
	constexpr const __m128 V_TWO           = MakeVector(  2.f,  2.f,  2.f,  2.f );
	constexpr const __m128 V_NEGTWO        = MakeVector( -2.f, -2.f, -2.f, -2.f );
	constexpr const __m128 V_HALF          = MakeVector( 0.5f, 0.5f, 0.5f, 0.5f );
	constexpr const __m128 V_PI            = MakeVector( PI, PI, PI, PI );
	constexpr const __m128 V_PI_2          = MakeVector( 0.5f * PI, 0.5f * PI, 0.5f * PI, 0.5f * PI );
	constexpr const __m128 V_PI2           = MakeVector( 2.0f * PI, 2.0f * PI, 2.0f * PI, 2.0f * PI );
	constexpr const __m128 V_ONE_PI2       = MakeVector( 1.f / (2.f * PI), 1.f / (2.f * PI), 1.f / (2.f * PI), 1.f / (2.f * PI) );
	constexpr const __m128 V_SIGNBIT       = MakeVector( unsigned int(1 << 31), unsigned int(1 << 31), unsigned int(1 << 31), unsigned int(1 << 31) );
	constexpr const __m128 V_SIGNMASK      = MakeVector( unsigned int(~(1 << 31)), unsigned int(~(1 << 31)), unsigned int(~(1 << 31)), unsigned int(~(1 << 31)) );

	constexpr const __m128 V_DEG_TO_RAD    = MakeVector( DEG_TO_RAD, DEG_TO_RAD, DEG_TO_RAD, DEG_TO_RAD );
	constexpr const __m128 V_RAD_TO_DEG    = MakeVector( RAD_TO_DEG, RAD_TO_DEG, RAD_TO_DEG, RAD_TO_DEG );
	constexpr const __m128 V_180F          = MakeVector( 180.f, 180.f, 180.f, 180.f);
	constexpr const __m128 V_360F          = MakeVector( 360.f, 360.f, 360.f, 360.f );
	
	constexpr const __m128 V_FLOAT_NON_FRACTIONAL = MakeVector( FLOAT_NON_FRACTIONAL, FLOAT_NON_FRACTIONAL, FLOAT_NON_FRACTIONAL, FLOAT_NON_FRACTIONAL );

	constexpr const __m128 V_XMASK         = MakeVector( 0xffffffffu, 0u, 0u, 0u );
	constexpr const __m128 V_YMASK         = MakeVector( 0u, 0xffffffffu, 0u, 0u );
	constexpr const __m128 V_ZMASK         = MakeVector( 0u, 0u, 0xffffffffu, 0u );
	constexpr const __m128 V_WMASK         = MakeVector( 0u, 0u, 0u, 0xffffffffu );
	constexpr const __m128 V_XYMASK        = MakeVector( 0xffffffffu, 0xffffffffu, 0u, 0u );
	constexpr const __m128 V_XZMASK        = MakeVector( 0xffffffffu, 0u, 0xffffffffu, 0u );
	constexpr const __m128 V_YWMASK        = MakeVector( 0u, 0xffffffffu, 0u, 0xffffffffu );
	constexpr const __m128 V_ZWMASK        = MakeVector( 0u, 0u, 0xffffffffu, 0xffffffffu );
	constexpr const __m128 V_XYZMASK       = MakeVector( 0xffffffffu, 0xffffffffu, 0xffffffffu, 0u );
	constexpr const __m128 V_YZWMASK       = MakeVector( 0u, 0xffffffffu, 0xffffffffu, 0xffffffffu );

	constexpr const __m128 V_255F          = MakeVector( 255.f, 255.f, 255.f, 255.f );
}


#ifndef SIMD_HPP_SSE_IMPL
#define SIMD_HPP_SSE_IMPL

	/**
	 * @brief Load a vector from aligned memory.
	 */
	#define VectorLoadAligned( ptr )                _mm_load_ps( (const float*)(ptr) )
	
	/**
	 * @brief Stores a vector to aligned memory.
	 */
	#define VectorStoreAligned( vec, ptr )          _mm_store_ps( (float*)(ptr), vec )

	/**
	 * @brief Mask a shuffle mask.
	 */
	#define ShuffleMask( a0, a1, b2, b3 )           ( (a0) | ((a1)<<2) | ((b2)<<4) | ((b3)<<6) )

	/**
	 * @brief Replicates one element into all four elements and returns the new vector.
	 * @return        ( Vec[index], Vec[index], Vec[index], Vec[index] )
	 */
	#define VectorReplicate( vec, index )           _mm_shuffle_ps( (vec), (vec), ShuffleMask( index, index, index, index ) )
	
	/**
	 * @brief Selects four specific values from vector via a mask.
	 */
	#define VectorPermute( vec, mask )              _mm_shuffle_ps( (vec), (vec), (mask) )
	
	/**
	 * @brief Swizzles the vector.
	 * @return        ( vec[x], vec[y], vec[z], vec[w] )
	 */
	#define VectorSwizzle( vec, x, y, z, w )        _mm_shuffle_ps( (vec), (vec), ShuffleMask( (x), (y), (z), (w) ) )

	/**
	 * @brief Creates a vector through selecting two elements from each vector via a shuffle mask.
	 * @return        ( vec1[x], vec1[y], vec2[z], vec2[w] )
	 */
	#define VectorShuffle( vec1, vec2, x, y, z, w ) _mm_shuffle_ps( (vec1), (vec2), ShuffleMask( (x), (y), (z), (w) ) )

	/**
	 * @brief Returns the negated vector.
	 * @return        ( -vec.x, -vec.y, -vec.z, -vec.w )
	 */
	#define VectorNegate( vec )                     _mm_sub_ps( _mm_setzero_ps(), (vec) )

	/**
	 * @brief Returns the minimum values of two vectors.
	 * @return        ( min( vec1.x, vec2.x ), same for yzw )
	 */
	#define VectorMin( vec1, vec2 )                 _mm_min_ps( vec1, vec2 )

	/**
	 * @brief Returns the maximum values of two vectors.
	 * @return        ( max( vec1.x, vec2.x ), same for yzw )
	 */
	#define VectorMax( vec1, vec2 )                 _mm_max_ps( vec1, vec2 )

	/**
	 * @brief Returns the absolute vector.
	 * @return        ( abs( vec1.x, vec2.x ), same for yzw )
	 */
	#define VectorAbs( vec )                        _mm_and_ps( (vec), Number::V_SIGNMASK )

	/**
	 * @brief Creates a four-part mask through elements comparison from each vector.
	 * @return        ( vec1.x == vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define VectorEQ( vec1, vec2 )                  _mm_cmpeq_ps( (vec1), (vec2) )
	
	/**
	 * @brief Creates a four-part mask through elements comparison from each vector.
	 * @return        ( vec1.x != vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define VectorNE( vec1, vec2 )                  _mm_cmpneq_ps( (vec1), (vec2) )
	
	/**
	 * @brief Creates a four-part mask through elements comparison from each vector.
	 * @return        ( vec1.x  > vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define VectorGT( vec1, vec2 )                  _mm_cmpgt_ps( (vec1), (vec2) )
	
	/**
	 * @brief Creates a four-part mask through elements comparison from each vector.
	 * @return        ( vec1.x >= vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define VectorGE( vec1, vec2 )                  _mm_cmpge_ps( (vec1), (vec2) )

	/**
	 * @brief Creates a four-part mask through elements comparison from each vector.
	 * @return        ( vec1.x  < vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define VectorLT( vec1, vec2 )                  _mm_cmplt_ps( (vec1), (vec2) )

	/**
	 * @brief Creates a four-part mask through elements comparison from each vector.
	 * @return        ( vec1.x <= vec2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */	
	#define VectorLE( vec1, vec2 )                  _mm_cmple_ps( (vec1), (vec2) )

	/**
	 * @brief Adds two vectors.
	 * @return        ( vec1.x + vec2.x, same for yzw )
	 */
	#define VectorAdd( vec1, vec2 )                 _mm_add_ps( (vec1), (vec2) )

	/**
	 * @brief Subtracts two vectors.
	 * @return        ( vec1.x - vec2.x, same for yzw )
	 */
	#define VectorSubtract( vec1, vec2 )            _mm_sub_ps( (vec1), (vec2) )

	/**
	 * @brief Multiplies two vectors.
	 * @return        ( vec1.x * vec2.x, same for yzw )
	 */
	#define VectorMultiply( vec1, vec2 )            _mm_mul_ps( (vec1), (vec2) )

	/**
	 * @brief Divides two vectors.
	 * @return        ( vec1.x / vec2.x, same for yzw )
	 */
	#define VectorDivide( vec1, vec2 )              _mm_div_ps( (vec1), (vec2) )

	/**
	 * @brief Multiplies two vectors, adds in the third vector.
	 * @return        ( vec1.x * vec2.x + vec3.x, same for yzw )
	 */
	#define VectorMultiplyAdd( vec1, vec2, vec3 )   _mm_add_ps( _mm_mul_ps( (vec1), (vec2) ), (vec3) )

	/**
	 * @brief Multiplies two vectors, then adds each result.
	 * @return        ( vec1.x * vec2.x + vec3.x * vec4.x, same for yzw )
	 */
	#define VectorMultiplyAddMultiply( vec1, vec2, vec3, vec4 )   _mm_add_ps( _mm_mul_ps( (vec1), (vec2) ), _mm_mul_ps( (vec3), (vec4) ) )

	/**
	 * @brief Combines two vectors using bitwise AND.
	 * @return        (vec1.x & vec2.x, same for yzw )
	 */
	#define VectorAnd( vec1, vec2 )                 _mm_and_ps( (vec1), (vec2) )

	/**
	 * @brief Combines two vectors using bitwise ANDNOT.
	 * @return        ( ~vec1.x & vec2.x, same for yzw )
	 */
	#define VectorAndNot( vec1, vec2 )              _mm_andnot_ps( (vec1), (vec2) )

	/**
	 * @brief Combines two vectors using bitwise OR.
	 * @return        ( vec1.x | vec2.x, same for yzw )
	 */
	#define VectorOr( vec1, vec2 )                  _mm_or_ps( (vec1), (vec2) )
	
	/**
	 * @brief Combines two vectors using bitwise OR.
	 * @return        ( vec1.x ^ vec2.x, same for yzw )
	 */
	#define VectorXor( vec1, vec2 )                 _mm_xor_ps( (vec1), (vec2) )

	/**
	 * @brief Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements in a vector.
	 * @return        Bit 0 = sign(mask.x), Bit 1 = sign(mask.y), Bit 2 = sign(mask.z), Bit 3 = sign(mask.w)
	 */
	#define VectorMaskBits( mask )                  _mm_movemask_ps( (mask) )

	/**
	 * @brief Keeps yzw, set x=0.
	 * @return        ( 0, vec.y, vec.z, vec.w )
	 */
	#define VectorSetX0( vec )                      _mm_and_ps( (vec), Number::V_YZWMASK )

	/**
	 * @brief Keeps xyz, set w=0.
	 * @return        ( vec.x, vec.y, vec.z, 0 )
	 */
	#define VectorSetW0( vec )                      _mm_and_ps( (vec), Number::V_XYZMASK )

	/**
	 * @biref Extracts x, ignore yzw.
	 * @return        ( vec.x )
	 */
	#define VectorGetX( vec )                       _mm_cvtss_f32( (vec) )

	/**
	 * @brief Convert float vector to signed 32-bit integer vector.
	 * @return        ( (int)vec.x, same for yzw )
	 */
	#define VectorCastInteger( vec )                _mm_cvtps_epi32( (vec) )

	/**
	 * @brief Convert signed 32-bit integer vector to float vector.
	 * @return        ( (float)vec.x, same for yzw )
	 */
	#define VectorCastFloat( vec )                _mm_cvtepi32_ps( (vec) )

	/**
	 * @brief Returns a bitwise equivalent vector based on 4 uint32.
	 */
	//force_inline __m128 MakeVector(const unsigned int& x, const unsigned int& y, const unsigned int& z, const unsigned int& w)
	//{
	//	union { __m128 v; __m128i i; } result;
	//	result.i = _mm_setr_epi32(x, y, z, w);
	//	return result.v;
	//}

	/**
	 * @brief Returns a vector based on 4 floats.
	 */
	force_inline __m128 MakeVector(const float& x, const float& y, const float& z, const float& w)
	{
		return _mm_setr_ps(x, y, z, w);
	}

	/**
	 * @brief Returns a signed 32-bits vector based on 4 integer.
	 */
	force_inline __m128i MakeVectorInteger(const int& x, const int& y, const int& z, const int& w)
	{
		return _mm_setr_epi32(x, y, z, w);
	}

	/**
	 * @brief Returns a vector based on 1 float.
	 */
	force_inline __m128 MakeVector(const float& val)
	{
		return _mm_set_ps1(val);
	}

	/**
	 * @brief copy each elements in a vector.
	 */
	force_inline void VectorCopy(const void* inVec, void* outVec) noexcept
	{
		VectorStoreAligned(VectorLoadAligned(inVec), outVec);
	}

	/**
	 * @brief Does a bitwise vector selection based on a mask.
	 * @return        ( for each bit i: Mask[i] ? vec1[i] : vec2[i] )
	 */
	force_inline __m128 VectorSelect(const __m128& mask, const __m128& vec1, const __m128& vec2)
	{
		return _mm_xor_ps(vec2, _mm_and_ps(mask, _mm_xor_ps(vec1, vec2)));
	}

	/**
	 * @brief Rounds A to an integer with truncation towards zero for each elements in a vector. (e.g. -1.7 truncated to -1, 2.8 truncated to 2)
	 * @return        ( truncate(vec.x), same for yzw )
	 */
	force_inline __m128 VectorTruncate(const __m128& vec)
	{
		return _mm_cvtepi32_ps(_mm_cvttps_epi32(vec));
	}

	/**
	 * @brief Returns a sum of four elements in a vector.
	 * @return        ( vec[0] + vec[1] + vec[2] + vec[3] )
	 */
	force_inline float VectorSum(const __m128& vec)
	{
		__m128 xy_zw = _mm_hadd_ps(vec, vec);
		__m128 xyzw = _mm_hadd_ps(xy_zw, xy_zw);
		return VectorGetX(xyzw);
	}

	/**
	 * @brief Returns a sum of four elements in a vector.
	 * @return        ( vec[0] + vec[1] + vec[2] + vec[3], same for yzw )
	 */
	force_inline __m128 VectorSum4(const __m128& vec)
	{
		__m128 xy_zw = _mm_hadd_ps(vec, vec);
		__m128 xyzw = _mm_hadd_ps(xy_zw, xy_zw);
		return xyzw;
	}

	/**
	 * @brief Computes a vector remainder of the division operation.
	 * @return        __m128( vec1.X % vec2.X,  same for yzw )
	 */
	force_inline __m128 VectorMod(const __m128& vec1, const __m128& vec2)
	{
		__m128 div = VectorDivide(vec1, vec2);

		// Floats where abs(f) >= 2^23 have no fractional portion, and larger values would overflow VectorTruncate.
		__m128 noFractionMask = VectorGE(VectorAbs(div), Number::V_FLOAT_NON_FRACTIONAL);
		__m128 temp = VectorSelect(noFractionMask, div, VectorTruncate(div));
		__m128 result = VectorSubtract(vec1, VectorMultiply(vec2, temp));

		// Clamp to [-abs2, abs2] because of possible failures for very large numbers (>1e10) due to precision loss.
		__m128 abs2 = VectorAbs(vec2);
		return VectorMax(VectorNegate(abs2), VectorMin(result, abs2));
	}

	/**
	 * @brief Calculate floor() for each elements in a vector.
	 * @return        ( floor(vec.x), same for yzw )
	 */
	force_inline __m128 VectorFloor(const __m128& vec)
	{
		__m128 trunc = VectorTruncate(vec);
		__m128 mask = VectorGE(vec, Number::V_ZERO);
		__m128 sub = VectorSelect(mask, Number::V_ZERO, Number::V_ONE);
		return VectorSubtract(trunc, sub);
	}

	/**
	 * @brief Calculate ceil() for each elements in a vector.
	 * @return        ( ceil(vec.x), same for yzw )
	 */
	force_inline __m128 VectorCeil(const __m128& vec)
	{
		__m128 trunc = VectorTruncate(vec);
		__m128 mask = VectorGE(vec, Number::V_ZERO);
		__m128 add = VectorSelect(mask, Number::V_ONE, Number::V_ZERO);
		return VectorAdd(trunc, add);
	}

#endif // !SIMD_HPP_SSE_IMPL



#ifndef SIMD_HPP_AVX_IMPL
#define SIMD_HPP_AVX_IMPL

	/**
	 * @brief Adds two vector8.
	 * @return        ( vec1.x1 + vec2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Vector8Add( vec1, vec2 )                 _mm256_add_ps( (vec1), (vec2) )

	/**
	 * @brief Subtracts two vector8.
	 * @return        ( vec1.x1 - vec2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Vector8Subtract( vec1, vec2 )            _mm256_sub_ps( (vec1), (vec2) )

	/**
	 * @brief Multiplies two vector8.
	 * @return        ( vec1.x1 * vec2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Vector8Multiply( vec1, vec2 )            _mm256_mul_ps( (vec1), (vec2) )

	/**
	 * @brief Divides two vector8.
	 * @return        ( vec1.x1 / vec2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Vector8Divide( vec1, vec2 )              _mm256_div_ps( (vec1), (vec2) )

	/**
	 * @brief Multiplies two vector8, adds in the third vector8.
	 * @return        ( vec1.x1 * vec2.x1 + vec3.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Vector8MultiplyAdd( vec1, vec2, vec3 )   _mm256_add_ps( _mm256_mul_ps( (vec1), (vec2) ), (vec3) )

	/**
	 * @brief Multiplies two vector8, then adds each result.
	 * @return        ( vec1.x1 * vec2.x1 + vec3.x1 * vec4.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Vector8MultiplyAddMultiply( vec1, vec2, vec3, vec4 )   _mm256_add_ps( _mm256_mul_ps( (vec1), (vec2) ), _mm256_mul_ps( (vec3), (vec4) ) )

	/**
	 * @brief Returns a vector8 based on 8 floats.
	 */
	force_inline __m256 MakeVector8(const float& x1, const float& y1, const float& z1, const float& w1, const float& x2, const float& y2, const float& z2, const float& w2)
	{
		return _mm256_setr_ps(x1, y1, z1, w1, x2, y2, z2, w2);
	}

	/**
	 * @brief Returns a vector8 based on 1 floats.
	 */
	force_inline __m256 MakeVector8(const float& val)
	{
		return _mm256_set1_ps(val);
	}

	/**
	 * @brief Returns a vector8 based on vector.
	 * @return        index == 0xffffffff ? ( vec, vec ) : ( vec[index], same for y1z1w1, x2y2z2w2 )
	 */
	template <unsigned int index = 0xffffffff>
	force_inline __m256 Vector8Replicate(const __m128& vec)
	{
		if constexpr (index == 0xffffffff)
		{
			__m256 result = _mm256_castps128_ps256(vec);
			return _mm256_insertf128_ps(result, vec, 1);
		}
		else
		{
			__m128 v = VectorReplicate(vec, index);
			__m256 result = _mm256_castps128_ps256(v);
			return _mm256_insertf128_ps(result, v, 1);
		}
	}


#endif // !SIMD_HPP_AVX_IMPL
