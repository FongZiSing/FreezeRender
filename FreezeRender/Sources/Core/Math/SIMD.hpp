//
// Math/SIMD.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Math functions of architecture intrinsic.
//
#pragma once

#include <Common.hpp>
#include <immintrin.h>
#include "Number.hpp"



namespace Pluto
{
	// 128-bit register.
	using R128 = __m128;

	// 128-bit integer register.
	using R128i = __m128i;

	// 128-bit double floating register.
	using R128d = __m128d;

	// 256-bit register.
	using R256 = __m256;

	// 256-bit integer register.
	using R256i = __m256i;

	// 256-bit double floating register.
	using R256d = __m256d;



	/**
	 * @brief Definitions of useful mathematical constants.
	 */
	namespace Number
	{
		force_inline constexpr R128 MakeRegister(const unsigned int& x, const unsigned int& y, const unsigned int& z, const unsigned int& w)
		{
			R128 result;
			result.m128_u32[0] = x;
			result.m128_u32[1] = y;
			result.m128_u32[2] = z;
			result.m128_u32[3] = w;
			return result;
		}

		force_inline constexpr R128 MakeRegister(const unsigned int& x)
		{
			R128 result;
			result.m128_u32[0] = x;
			result.m128_u32[1] = x;
			result.m128_u32[2] = x;
			result.m128_u32[3] = x;
			return result;
		}

		force_inline constexpr R128 MakeRegister(const float& x, const float& y, const float& z, const float& w)
		{
			R128 result;
			result.m128_f32[0] = x;
			result.m128_f32[1] = y;
			result.m128_f32[2] = z;
			result.m128_f32[3] = w;
			return result;
		}

		force_inline constexpr R128 MakeRegister(const float& x)
		{
			R128 result;
			result.m128_f32[0] = x;
			result.m128_f32[1] = x;
			result.m128_f32[2] = x;
			result.m128_f32[3] = x;
			return result;
		}

		force_inline constexpr R128d MakeRegister(const double& x, const double& y)
		{
			R128d result{};
			result.m128d_f64[0] = x;
			result.m128d_f64[1] = y;
			return result;
		}

		force_inline constexpr __m128d MakeRegister(const double& x)
		{
			R128d result{};
			result.m128d_f64[0] = x;
			result.m128d_f64[1] = x;
			return result;
		}

		//--------------------------------
		//~ Begin 4-floating constants.

		constexpr const R128 R_ZERO          = MakeRegister(  0.f );
		constexpr const R128 R_ONE           = MakeRegister(  1.f );
		constexpr const R128 R_NEGONE        = MakeRegister( -1.f );
		constexpr const R128 R_TWO           = MakeRegister(  2.f );
		constexpr const R128 R_NEGTWO        = MakeRegister( -2.f );
		constexpr const R128 R_HALF          = MakeRegister( 0.5f );
		constexpr const R128 R_PI            = MakeRegister(  PI  );
		constexpr const R128 R_PI_2          = MakeRegister( 0.5f * PI );
		constexpr const R128 R_PI2           = MakeRegister( 2.0f * PI );
		constexpr const R128 R_ONE_PI2       = MakeRegister( 1.f / (2.f * PI) );
		constexpr const R128 R_SIGNBIT       = MakeRegister( unsigned int(1 << 31) );
		constexpr const R128 R_SIGNMASK      = MakeRegister( unsigned int(~(1 << 31)) );

		constexpr const R128 R_DEG_TO_RAD    = MakeRegister( DEG_TO_RAD );
		constexpr const R128 R_RAD_TO_DEG    = MakeRegister( RAD_TO_DEG );
		constexpr const R128 R_180F          = MakeRegister( 180.f );
		constexpr const R128 R_360F          = MakeRegister( 360.f );
	
		constexpr const R128 R_FLOAT_NON_FRACTIONAL = MakeRegister( FLOAT_NON_FRACTIONAL );

		constexpr const R128 R_XMASK         = MakeRegister( 0xffffffffu,     0u,          0u,          0u      );
		constexpr const R128 R_YMASK         = MakeRegister(     0u,      0xffffffffu,     0u,          0u      );
		constexpr const R128 R_ZMASK         = MakeRegister(     0u,          0u,      0xffffffffu,     0u      );
		constexpr const R128 R_WMASK         = MakeRegister(     0u,          0u,          0u,      0xffffffffu );
		constexpr const R128 R_XYMASK        = MakeRegister( 0xffffffffu, 0xffffffffu,     0u,          0u      );
		constexpr const R128 R_XZMASK        = MakeRegister( 0xffffffffu,     0u,      0xffffffffu,     0u      );
		constexpr const R128 R_YWMASK        = MakeRegister(     0u,      0xffffffffu,     0u,      0xffffffffu );
		constexpr const R128 R_ZWMASK        = MakeRegister(     0u,          0u,      0xffffffffu, 0xffffffffu );
		constexpr const R128 R_XYZMASK       = MakeRegister( 0xffffffffu, 0xffffffffu, 0xffffffffu,     0u      );
		constexpr const R128 R_YZWMASK       = MakeRegister(     0u,      0xffffffffu, 0xffffffffu, 0xffffffffu );

		constexpr const R128 R_255F          = MakeRegister( 255.f );

		//~ End 4-floating constants.
		//--------------------------------


		
		//--------------------------------
		//~ Begin 2-floating constants.

		constexpr const R128d R_ZEROd = MakeRegister(0.0);
		constexpr const R128d R_ONEd  = MakeRegister(1.0);
		constexpr const R128d R_HALFd = MakeRegister(0.5);

		//~ End 4-floating constants.
		//--------------------------------
	}



#pragma region sse

	/**
	 * @brief Load a R128 from aligned memory.
	 */
	#define RegisterLoadAligned( ptr )                _mm_load_ps( (const float*)(ptr) )
	
	/**
	 * @brief Stores a R128 to aligned memory.
	 */
	#define RegisterStoreAligned( reg, ptr )          _mm_store_ps( (float*)(ptr), reg )

	/**
	 * @brief Mask a shuffle mask.
	 */
	#define ShuffleMask( a0, a1, b2, b3 )           ( (a0) | ((a1)<<2) | ((b2)<<4) | ((b3)<<6) )

	/**
	 * @brief Replicates one element into all four elements and returns the new R128.
	 * @return        ( reg[index], reg[index], reg[index], reg[index] )
	 */
	#define RegisterReplicate( reg, index )           _mm_shuffle_ps( (reg), (reg), ShuffleMask( index, index, index, index ) )
	
	/**
	 * @brief Selects four specific values from R128 via a mask.
	 */
	#define RegisterPermute( reg, mask )              _mm_shuffle_ps( (reg), (reg), (mask) )
	
	/**
	 * @brief Swizzles the R128.
	 * @return        ( reg[x], reg[y], reg[z], reg[w] )
	 */
	#define RegisterSwizzle( reg, x, y, z, w )        _mm_shuffle_ps( (reg), (reg), ShuffleMask( (x), (y), (z), (w) ) )

	/**
	 * @brief Creates a R128 through selecting two elements from each R128 via a shuffle mask.
	 * @return        ( reg1[x], reg1[y], reg2[z], reg2[w] )
	 */
	#define RegisterShuffle( reg1, reg2, x, y, z, w ) _mm_shuffle_ps( (reg1), (reg2), ShuffleMask( (x), (y), (z), (w) ) )

	/**
	 * @brief Returns the negated R128.
	 * @return        ( -reg.x, -reg.y, -reg.z, -reg.w )
	 */
	#define RegisterNegate( reg )                     _mm_sub_ps( _mm_setzero_ps(), (reg) )

	/**
	 * @brief Returns the minimum values of two R128.
	 * @return        ( min( reg1.x, reg2.x ), same for yzw )
	 */
	#define RegisterMin( reg1, reg2 )                 _mm_min_ps( reg1, reg2 )

	/**
	 * @brief Returns the maximum values of two R128.
	 * @return        ( max( reg1.x, reg2.x ), same for yzw )
	 */
	#define RegisterMax( reg1, reg2 )                 _mm_max_ps( reg1, reg2 )

	/**
	 * @brief Returns the absolute R128.
	 * @return        ( abs( reg1.x, reg2.x ), same for yzw )
	 */
	#define RegisterAbs( reg )                        _mm_and_ps( (reg), Number::R_SIGNMASK )

	/**
	 * @brief Creates a four-part mask through elements comparison from each R128.
	 * @return        ( reg1.x == reg2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define RegisterEQ( reg1, reg2 )                  _mm_cmpeq_ps( (reg1), (reg2) )
	
	/**
	 * @brief Creates a four-part mask through elements comparison from each R128.
	 * @return        ( reg1.x != reg2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define RegisterNE( reg1, reg2 )                  _mm_cmpneq_ps( (reg1), (reg2) )
	
	/**
	 * @brief Creates a four-part mask through elements comparison from each R128.
	 * @return        ( reg1.x  > reg2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define RegisterGT( reg1, reg2 )                  _mm_cmpgt_ps( (reg1), (reg2) )
	
	/**
	 * @brief Creates a four-part mask through elements comparison from each R128.
	 * @return        ( reg1.x >= reg2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define RegisterGE( reg1, reg2 )                  _mm_cmpge_ps( (reg1), (reg2) )

	/**
	 * @brief Creates a four-part mask through elements comparison from each R128.
	 * @return        ( reg1.x  < reg2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */
	#define RegisterLT( reg1, reg2 )                  _mm_cmplt_ps( (reg1), (reg2) )

	/**
	 * @brief Creates a four-part mask through elements comparison from each R128.
	 * @return        ( reg1.x <= reg2.x ? 0xFFFFFFFF : 0, same for yzw )
	 */	
	#define RegisterLE( reg1, reg2 )                  _mm_cmple_ps( (reg1), (reg2) )

	/**
	 * @brief Adds two R128.
	 * @return        ( reg1.x + reg2.x, same for yzw )
	 */
	#define RegisterAdd( reg1, reg2 )                 _mm_add_ps( (reg1), (reg2) )

	/**
	 * @brief Subtracts two R128.
	 * @return        ( reg1.x - reg2.x, same for yzw )
	 */
	#define RegisterSubtract( reg1, reg2 )            _mm_sub_ps( (reg1), (reg2) )

	/**
	 * @brief Multiplies two R128.
	 * @return        ( reg1.x * reg2.x, same for yzw )
	 */
	#define RegisterMultiply( reg1, reg2 )            _mm_mul_ps( (reg1), (reg2) )

	/**
	 * @brief Divides two R128.
	 * @return        ( reg1.x / reg2.x, same for yzw )
	 */
	#define RegisterDivide( reg1, reg2 )              _mm_div_ps( (reg1), (reg2) )

	/**
	 * @brief Multiplies two R128, adds in the third R128.
	 * @return        ( reg1.x * reg2.x + reg3.x, same for yzw )
	 */
	#define RegisterMultiplyAdd( reg1, reg2, reg3 )   _mm_add_ps( _mm_mul_ps( (reg1), (reg2) ), (reg3) )

	/**
	 * @brief Multiplies two R128, then adds each result.
	 * @return        ( reg1.x * reg2.x + reg3.x * reg4.x, same for yzw )
	 */
	#define RegisterMultiplyAddMultiply( reg1, reg2, reg3, reg4 )   _mm_add_ps( _mm_mul_ps( (reg1), (reg2) ), _mm_mul_ps( (reg3), (reg4) ) )

	/**
	 * @brief Combines two R128 using bitwise AND.
	 * @return        (reg1.x & reg2.x, same for yzw )
	 */
	#define RegisterAnd( reg1, reg2 )                 _mm_and_ps( (reg1), (reg2) )

	/**
	 * @brief Combines two R128 using bitwise ANDNOT.
	 * @return        ( ~reg1.x & reg2.x, same for yzw )
	 */
	#define RegisterAndNot( reg1, reg2 )              _mm_andnot_ps( (reg1), (reg2) )

	/**
	 * @brief Combines two R128 using bitwise OR.
	 * @return        ( reg1.x | reg2.x, same for yzw )
	 */
	#define RegisterOr( reg1, reg2 )                  _mm_or_ps( (reg1), (reg2) )
	
	/**
	 * @brief Combines two R128 using bitwise OR.
	 * @return        ( reg1.x ^ reg2.x, same for yzw )
	 */
	#define RegisterXor( reg1, reg2 )                 _mm_xor_ps( (reg1), (reg2) )

	/**
	 * @brief Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements in a R128.
	 * @return        Bit 0 = sign(mask.x), Bit 1 = sign(mask.y), Bit 2 = sign(mask.z), Bit 3 = sign(mask.w)
	 */
	#define RegisterMaskBits( mask )                  _mm_movemask_ps( (mask) )

	/**
	 * @brief Keeps yzw, set x=0.
	 * @return        ( 0, reg.y, reg.z, reg.w )
	 */
	#define RegisterSetX0( reg )                      _mm_and_ps( (reg), Number::R_YZWMASK )

	/**
	 * @brief Keeps xyz, set w=0.
	 * @return        ( reg.x, reg.y, reg.z, 0 )
	 */
	#define RegisterSetW0( reg )                      _mm_and_ps( (reg), Number::R_XYZMASK )

	/**
	 * @biref Extracts x, ignore yzw.
	 * @return        ( reg.x )
	 */
	#define RegisterGetX( reg )                       _mm_cvtss_f32( (reg) )

	/**
	 * @brief Convert float R128 to signed 32-bit integer R128.
	 * @return        ( (int)reg.x, same for yzw )
	 */
	#define RegisterCastInteger( reg )                _mm_cvtps_epi32( (reg) )

	/**
	 * @brief Convert signed 32-bit integer R128 to float R128.
	 * @return        ( (float)reg.x, same for yzw )
	 */
	#define RegisterCastFloat( reg )                  _mm_cvtepi32_ps( (reg) )

	/**
	 * @brief Returns a R128 based on 4 floats.
	 */
	force_inline R128 MakeRegister(const float& x, const float& y, const float& z, const float& w)
	{
		return _mm_setr_ps(x, y, z, w);
	}

	/**
	 * @brief Returns a signed 32-bits R128 based on 4 integer.
	 */
	force_inline R128i MakeRegisterInteger(const int& x, const int& y, const int& z, const int& w)
	{
		return _mm_setr_epi32(x, y, z, w);
	}

	/**
	 * @brief Returns a R128 based on 1 float.
	 */
	force_inline R128 MakeRegister(const float& val)
	{
		return _mm_set_ps1(val);
	}

	/**
	 * @brief copy 4 floats.
	 */
	force_inline void RegisterCopy(const void* src, void* dst) noexcept
	{
		_mm_store_ps((float*)(dst), *(__m128*)(src));
		//RegisterStoreAligned(RegisterLoadAligned(src), dst);
	}

	/**
	 * @brief Does a bitwise R128 selection based on a mask.
	 * @return        ( for each bit i: Mask[i] ? reg1[i] : reg2[i] )
	 */
	force_inline R128 RegisterSelect(const R128& mask, const R128& reg1, const R128& reg2)
	{
		return _mm_xor_ps(reg2, _mm_and_ps(mask, _mm_xor_ps(reg1, reg2)));
	}

	/**
	 * @brief Rounds A to an integer with truncation towards zero for each elements in a R128. (e.g. -1.7 truncated to -1, 2.8 truncated to 2)
	 * @return        ( truncate(reg.x), same for yzw )
	 */
	force_inline R128 RegisterTruncate(const R128& reg)
	{
		return _mm_cvtepi32_ps(_mm_cvttps_epi32(reg));
	}

	/**
	 * @brief Returns a sum of four elements in a R128.
	 * @return        ( reg[0] + reg[1] + reg[2] + reg[3] )
	 */
	force_inline float RegisterSum(const R128& reg)
	{
		R128 xy_zw = _mm_hadd_ps(reg, reg);
		R128 xyzw = _mm_hadd_ps(xy_zw, xy_zw);
		return RegisterGetX(xyzw);
	}

	/**
	 * @brief Returns a sum of four elements in a R128.
	 * @return        ( reg[0] + reg[1] + reg[2] + reg[3], same for yzw )
	 */
	force_inline R128 RegisterSum4(const R128& reg)
	{
		R128 xy_zw = _mm_hadd_ps(reg, reg);
		R128 xyzw = _mm_hadd_ps(xy_zw, xy_zw);
		return xyzw;
	}

	/**
	 * @brief Computes a R128 remainder of the division operation.
	 * @return        R128( reg1.X % reg2.X,  same for yzw )
	 */
	force_inline R128 RegisterMod(const R128& reg1, const R128& reg2)
	{
		R128 div = RegisterDivide(reg1, reg2);

		// Floats where abs(f) >= 2^23 have no fractional portion, and larger values would overflow RegisterTruncate.
		R128 noFractionMask = RegisterGE(RegisterAbs(div), Number::R_FLOAT_NON_FRACTIONAL);
		R128 temp = RegisterSelect(noFractionMask, div, RegisterTruncate(div));
		R128 result = RegisterSubtract(reg1, RegisterMultiply(reg2, temp));

		// Clamp to [-abs2, abs2] because of possible failures for very large numbers (>1e10) due to precision loss.
		R128 abs2 = RegisterAbs(reg2);
		return RegisterMax(RegisterNegate(abs2), RegisterMin(result, abs2));
	}

	/**
	 * @brief Calculate floor() for each elements in a R128.
	 * @return        ( floor(reg.x), same for yzw )
	 */
	force_inline R128 RegisterFloor(const R128& reg)
	{
		R128 trunc = RegisterTruncate(reg);
		R128 mask = RegisterGE(reg, Number::R_ZERO);
		R128 sub = RegisterSelect(mask, Number::R_ZERO, Number::R_ONE);
		return RegisterSubtract(trunc, sub);
	}

	/**
	 * @brief Calculate ceil() for each elements in a R128.
	 * @return        ( ceil(reg.x), same for yzw )
	 */
	force_inline R128 RegisterCeil(const R128& reg)
	{
		R128 trunc = RegisterTruncate(reg);
		R128 mask = RegisterGE(reg, Number::R_ZERO);
		R128 add = RegisterSelect(mask, Number::R_ONE, Number::R_ZERO);
		return RegisterAdd(trunc, add);
	}

#pragma endregion sse



#pragma region avx

	/**
	 * @brief Load a R256 from aligned memory.
	 */
	#define Register8LoadAligned( ptr )                _mm256_load_ps( (const float*)(ptr) )
	
	/**
	 * @brief Stores a R256 to aligned memory.
	 */
	#define Register8StoreAligned( reg, ptr )          _mm256_store_ps( (float*)(ptr), reg )

	/**
	 * @brief Copy 8 floats.
	 */
	#define Register8Copy( src, dst )                  _mm256_store_ps( (float*)(dst), *(__m256*)(src) )

	/**
	 * @brief set zero.
	 */
	#define Register8SetZero( dst )                    _mm256_store_ps( (float*)(dst), _mm256_setzero_ps() )

	/**
	 * @brief Adds two R256.
	 * @return        ( reg1.x1 + reg2.x1, same for y1z1w1, x2y2z2w2 )
	 */

	#define Register8Add( reg1, reg2 )                 _mm256_add_ps( (reg1), (reg2) )

	/**
	 * @brief Subtracts two R256.
	 * @return        ( reg1.x1 - reg2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Register8Subtract( reg1, reg2 )            _mm256_sub_ps( (reg1), (reg2) )

	/**
	 * @brief Multiplies two R256.
	 * @return        ( reg1.x1 * reg2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Register8Multiply( reg1, reg2 )            _mm256_mul_ps( (reg1), (reg2) )

	/**
	 * @brief Divides two R256.
	 * @return        ( reg1.x1 / reg2.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Register8Divide( reg1, reg2 )              _mm256_div_ps( (reg1), (reg2) )

	/**
	 * @brief Multiplies two R256, adds in the third R256.
	 * @return        ( reg1.x1 * reg2.x1 + reg3.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Register8MultiplyAdd( reg1, reg2, reg3 )   _mm256_add_ps( _mm256_mul_ps( (reg1), (reg2) ), (reg3) )

	/**
	 * @brief Multiplies two R256, then adds each result.
	 * @return        ( reg1.x1 * reg2.x1 + reg3.x1 * reg4.x1, same for y1z1w1, x2y2z2w2 )
	 */
	#define Register8MultiplyAddMultiply( reg1, reg2, reg3, reg4 )   _mm256_add_ps( _mm256_mul_ps( (reg1), (reg2) ), _mm256_mul_ps( (reg3), (reg4) ) )

	/**
	 * @brief Returns a R256 based on 8 floats.
	 */
	force_inline R256 MakeRegister8(const float& x1, const float& y1, const float& z1, const float& w1, const float& x2, const float& y2, const float& z2, const float& w2)
	{
		return _mm256_setr_ps(x1, y1, z1, w1, x2, y2, z2, w2);
	}

	/**
	 * @brief Returns a R256 based on 1 floats.
	 */
	force_inline R256 MakeRegister8(const float& val)
	{
		return _mm256_set1_ps(val);
	}

	/**
	 * @brief Returns a R256 based on R128.
	 * @return        index == 0xffffffff ? ( reg, reg ) : ( reg[index], same for y1z1w1, x2y2z2w2 )
	 */
	template <unsigned int index = 0xffffffff>
	force_inline R256 Register8Replicate(const R128& reg)
	{
		if constexpr (index == 0xffffffff)
		{
			R256 result = _mm256_castps128_ps256(reg);
			return _mm256_insertf128_ps(result, reg, 1);
		}
		else
		{
			R128 v = RegisterReplicate(reg, index);
			R256 result = _mm256_castps128_ps256(v);
			return _mm256_insertf128_ps(result, v, 1);
		}
	}

	/**
	 * @brief Load a R256d from aligned memory.
	 */
	#define Register4dLoadAligned( ptr )                _mm256_load_pd( (const double*)(ptr) )
	
	/**
	 * @brief Stores a R256d to aligned memory.
	 */
	#define Register4dStoreAligned( reg, ptr )          _mm256_store_pd( (double*)(ptr), reg )

	/**
	 * @brief Copy 4 double floating-point.
	 */
	#define Register4dCopy( src, dst )                  _mm256_store_pd( (double*)(dst), *(__m256d*)(src) )

	/**
	 * @brief set zero.
	 */
	#define Register4dSetZero( dst )                    _mm256_store_pd( (double*)(dst), _mm256_setzero_pd() )

	/**
	 * @brief Adds two R256d.
	 * @return        ( reg1.x + reg2.x, same for yzw )
	 */

	#define Register4dAdd( reg1, reg2 )                 _mm256_add_pd( (reg1), (reg2) )

	/**
	 * @brief Subtracts two R256d.
	 * @return        ( reg1.x - reg2.x, same for yzw )
	 */
	#define Register4dSubtract( reg1, reg2 )            _mm256_sub_pd( (reg1), (reg2) )

	/**
	 * @brief Multiplies two R256d.
	 * @return        ( reg1.x * reg2.x, same for yzw )
	 */
	#define Register4dMultiply( reg1, reg2 )            _mm256_mul_pd( (reg1), (reg2) )

	/**
	 * @brief Divides two R256d.
	 * @return        ( reg1.x / reg2.x, same for yzw )
	 */
	#define Register4dDivide( reg1, reg2 )              _mm256_div_pd( (reg1), (reg2) )

	/**
	 * @brief Multiplies two R256d, adds in the third R256d.
	 * @return        ( reg1.x * reg2.x + reg3.x, same for yzw )
	 */
	#define Register4dMultiplyAdd( reg1, reg2, reg3 )   _mm256_add_pd( _mm256_mul_pd( (reg1), (reg2) ), (reg3) )

	/**
	 * @brief Multiplies two R256d, then adds each result.
	 * @return        ( reg1.x * reg2.x + reg3.x * reg4.x, same for yzw )
	 */
	#define Register4dMultiplyAddMultiply( reg1, reg2, reg3, reg4 )   _mm256_add_pd( _mm256_mul_pd( (reg1), (reg2) ), _mm256_mul_pd( (reg3), (reg4) ) )

	 /**
	  * @brief Replicates one element into all four elements and returns the new R256d.
	  * @return        ( reg[index], reg[index], reg[index], reg[index] )
	  */
	
	template <int index>
	force_inline R256d Register4dReplicate(const R256d& reg)
	{
		if constexpr (index == 0)
		{

			R128d temp = _mm256_extractf128_pd(_mm256_movedup_pd(reg), 0);
			R256d result = _mm256_broadcast_pd(&temp);
			return result;
		}
		else if constexpr (index == 1)
		{
			R128d temp = _mm256_extractf128_pd(_mm256_permute_pd(reg, 0b1111), 0);
			R256d result = _mm256_broadcast_pd(&temp);
			return result;
		}
		else if constexpr (index == 2)
		{
			R128d temp = _mm256_extractf128_pd(_mm256_permute_pd(reg, 0), 1);
			R256d result = _mm256_broadcast_pd(&temp);
			return result;
		}
		else if constexpr (index == 3)
		{
			R128d temp = _mm256_extractf128_pd(_mm256_permute_pd(reg, 0b1111), 1);
			R256d result = _mm256_broadcast_pd(&temp);
			return result;
		}
	}


#pragma endregion sse
}
