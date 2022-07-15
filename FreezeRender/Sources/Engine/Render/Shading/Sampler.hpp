//
// Sampler.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of texture sampler.
//
#pragma once

#include <Common.hpp>
#include <Asset/Texture/Texture.hpp>



namespace Pluto
{
	/**
	 * @brief The Detail implemention of sampler.
	 */
	using AssetTexture = ATexture;
	using AddressMode = AssetTexture::AddressMode;
	using FilterMode = AssetTexture::FilterMode;
	using Sampler = Color(*)(const AssetTexture* target, const float& u, const float& v);

	/**
	 * @brief The root entry of nearest-sampler.
	 */
	template<AddressMode AddressX, AddressMode AddressY = AddressX>
	Color NearestSampler(const AssetTexture* target, const float& u, const float& v)
	{
		float fixedu, fixedv;
		if constexpr (AddressX == AddressMode::Warp)
		{
			// Limit to range [0, 1], rounded towards zero.
			fixedu = u - std::floorf(u);
		}
		else if constexpr (AddressX == AddressMode::Mirror)
		{
			// Limit to range [0, 2], rounded toward zero.
			const float halfu = u / 2.f;
			const float modu = (halfu - std::floorf(halfu)) * 2.f;

			// f(u) = 1 - | u - 1 |
			fixedu = 1 - std::fabsf(modu - 1.f);
		}
		else if constexpr (AddressX == AddressMode::Clamp)
		{
			// Clamp.
			fixedu = Math::Clamp(0.f, u, 1.f);
		}
		else if constexpr (AddressX == AddressMode::Border)
		{
			// Limit.
			if (u >= 0.f && u <= 1.f)
				fixedu = u;
			else
				return target->border;
		}
		else
		{
			//static_assert(false, "[FreezeRender] Unknown address x mode!");
		}

		if constexpr (AddressY == AddressMode::Warp)
		{
			// Limit to range [0, 1], rounded towards zero.
			fixedv = 1.f - (v - std::floorf(v));
		}
		else if constexpr (AddressY == AddressMode::Mirror)
		{
			// Limit to range [0, 2], rounded toward zero.
			const float halfv = v / 2.f;
			const float modv = (1.f - (halfv - std::floorf(halfv))) * 2.f;

			// f(v) = 1 - | v - 1 |
			fixedv = 1 - std::fabsf(modv - 1.f);
		}
		else if constexpr (AddressY == AddressMode::Clamp)
		{
			// Clamp.
			fixedv = 1.f - Math::Clamp(0.f, v, 1.f);
		}
		else if constexpr (AddressY == AddressMode::Border)
		{
			if (v >= 0.f && v <= 1.f)
			{
				fixedv = 1.f - v;
			}
			else
			{
				return target->border;
			}
		}
		else
		{
			//static_assert(false, "[FreezeRender] Unknown address y mode!");
		}

		// Get position of texel.
		const int x = static_cast<int>(fixedu * target->width) % target->width;
		const int y = static_cast<int>(fixedv * target->height) % target->height;
		const unsigned char* texel = target->bits.Get() + y * target->strides + x * target->bytePerPixel;

		return { texel[0], texel[1], texel[2], texel[3] };
	}

	/**
		* @brief The root entry of bilinear-sampler.
		*/
	template<AddressMode AddressX, AddressMode AddressY = AddressX>
	Color BilinearSampler(const AssetTexture* target, const float& u, const float& v)
	{
		R128 alphax, alphay;
		int x0, x1, y0, y1;
		bool xborder = false, yborder = false;
		if constexpr (AddressX == AddressMode::Warp)
		{
			// Limit to range [0, 1], rounded towards zero.
			const float fixedu = u - std::floorf(u);

			// Get position of texel.
			const int width = target->width;
			const float realx = fixedu * width;
			x0 = static_cast<int>(realx) % width;
			x1 = (x0 + 1) % width;

			// Get interpolation coefficient.
			alphax = MakeRegister(realx - x0);
		}
		else if constexpr (AddressX == AddressMode::Mirror)
		{
			// Limit to range [0, 2], rounded toward zero.
			R128 u2 = MakeRegister(u, u, u + 1.f / target->width, u + 1.f / target->width);
			R128 fixedu = RegisterMultiply(u2, Number::R_HALF);
			fixedu = RegisterMultiply(RegisterSubtract(fixedu, RegisterFloor(fixedu)), Number::R_TWO);

			// f(u) = 1 - | u - 1 |
			fixedu = RegisterSubtract(Number::R_ONE, RegisterAbs(RegisterSubtract(fixedu, Number::R_ONE)));

			// Get location of texel.
			R128 width = MakeRegister(target->width - 1.f);
			R128 realx = RegisterMultiply(fixedu, width);
			R128 maskx = RegisterGT(RegisterReplicate(realx, 0), RegisterReplicate(realx, 2));

			R128i x2 = RegisterCastInteger(RegisterFloor(realx));
			x0 = x2.m128i_i32[0];
			x1 = x2.m128i_i32[2];

			// Get interpolation coefficient.
			R128 alpha = RegisterSubtract(realx, RegisterCastFloat(x2));
			alphax = RegisterReplicate(alpha, 0);
			alphax = RegisterSelect(maskx, RegisterSubtract(Number::R_ONE, alphax), alphax);
		}
		else if constexpr (AddressX == AddressMode::Clamp)
		{
			// Limit to range [0, 1], rounded towards zero.
			const float fixedu = Math::Clamp(0.f, u, 1.f);

			// Get position of texel.
			const int width = target->width;
			const float realx = fixedu * width;
			x0 = Math::Clamp(0, static_cast<int>(realx), width - 1);
			x1 = Math::Clamp(0, x0 + 1, width - 1);

			// Get interpolation coefficient.
			alphax = MakeRegister(realx - x0);
		}
		else if constexpr (AddressX == AddressMode::Border)
		{
			if (u >= 0 && u <= 1.f)
			{
				// Get position of texel.
				const int width = target->width;
				const float realx = u * width;
				x0 = Math::Clamp(0, static_cast<int>(realx), width - 1);
				x1 = Math::Clamp(0, x0 + 1, width - 1);
				xborder = x0 == x1;

				// Get interpolation coefficient.
				alphax = MakeRegister(realx - x0);
			}
			else
			{
				return target->border;
			}
		}
		else
		{
			//static_assert(false, "[FreezeRender] Unknown address x mode!");
		}

		if constexpr (AddressY == AddressMode::Warp)
		{
			// Limit to range [0, 1], rounded towards zero.
			const float fixedv = 1.f - (v - std::floorf(v));

			// Get position of texel.
			const int height = target->height;
			const float realy = fixedv * height;
			y0 = static_cast<int>(realy) % height;
			y1 = (y0 + 1) % height;

			// Get interpolation coefficient.
			alphay = MakeRegister(realy - y0);
		}
		else if constexpr (AddressY == AddressMode::Mirror)
		{
			// Limit to range [0, 2], rounded toward zero.
			R128 v2 = MakeRegister(v, v, v + 1.f / target->height, v + 1.f / target->height);
			R128 fixedv = RegisterMultiply(v2, Number::R_HALF);
			fixedv = RegisterSubtract(fixedv, RegisterFloor(fixedv));
			fixedv = RegisterMultiply(RegisterSubtract(Number::R_ONE, fixedv), Number::R_TWO);

			// f(v) = 1 - | v - 1 |
			fixedv = RegisterSubtract(Number::R_ONE, RegisterAbs(RegisterSubtract(fixedv, Number::R_ONE)));

			// Get location of texel.
			R128 height = MakeRegister(target->height - 1.f);
			R128 realy = RegisterMultiply(fixedv, height);
			R128 masky = RegisterGT(RegisterReplicate(realy, 0), RegisterReplicate(realy, 2));

			R128i y2 = RegisterCastInteger(RegisterFloor(realy));
			y0 = y2.m128i_i32[0];
			y1 = y2.m128i_i32[2];

			// Get interpolation coefficient.
			R128 alpha = RegisterSubtract(realy, RegisterCastFloat(y2));
			alphay = RegisterReplicate(alpha, 0);
			alphay = RegisterSelect(masky, RegisterSubtract(Number::R_ONE, alphay), alphay);
		}
		else if constexpr (AddressY == AddressMode::Clamp)
		{
			// Limit to range [0, 1], rounded towards zero.
			const float fixedv = 1.f - Math::Clamp(0.f, v, 1.f);

			// Get position of texel.
			const int height = target->height;
			const float realy = fixedv * height;
			y0 = Math::Clamp(0, static_cast<int>(realy), height - 1);
			y1 = Math::Clamp(0, y0 + 1, height - 1);

			// Get interpolation coefficient.
			alphay = MakeRegister(realy - y0);
		}
		else if constexpr (AddressY == AddressMode::Border)
		{
			if (v >= 0 && v <= 1.f)
			{
				// Get position of texel.
				const int height = target->height;
				const float realy = (1.f - v) * height;
				y0 = Math::Clamp(0, static_cast<int>(realy), height - 1);
				y1 = Math::Clamp(0, y0 + 1, height - 1);
				yborder = y0 == y1;

				// Get interpolation coefficient.
				alphay = MakeRegister(realy - y0);
			}
			else
			{
				return target->border;
			}
		}
		else
		{
			//static_assert(false, "[FreezeRender] Unknown address y mode!");
		}

		// Get a piece of color.
		R128 texel00, texel01, texel10, texel11;
		unsigned char* color = target->bits.Get(y0 * target->strides + x0 * target->bytePerPixel);
		texel00 = MakeRegister(float(color[0]), float(color[1]), float(color[2]), float(color[3]));

		color = xborder ? (unsigned char*)(&target->border) : target->bits.Get(y0 * target->strides + x1 * target->bytePerPixel);
		texel01 = MakeRegister(float(color[0]), float(color[1]), float(color[2]), float(color[3]));

		color = yborder ? (unsigned char*)(&target->border) : target->bits.Get(y1 * target->strides + x0 * target->bytePerPixel);
		texel10 = MakeRegister(float(color[0]), float(color[1]), float(color[2]), float(color[3]));

		color = xborder && yborder ? (unsigned char*)(&target->border) : target->bits.Get(y1 * target->strides + x1 * target->bytePerPixel);
		texel11 = MakeRegister(float(color[0]), float(color[1]), float(color[2]), float(color[3]));

		// interpolation.
		R128 tempx0 = RegisterMultiplyAdd(alphax, RegisterSubtract(texel01, texel00), texel00);
		R128 tempx1 = RegisterMultiplyAdd(alphax, RegisterSubtract(texel11, texel10), texel10);
		R128 temp = RegisterMultiplyAdd(alphay, RegisterSubtract(tempx1, tempx0), tempx0);

		// Clamp to [0, 255].
		temp = RegisterSelect(RegisterGT(temp, Number::R_255F), Number::R_255F, temp);
		temp = RegisterSelect(RegisterLT(temp, Number::R_ZERO), Number::R_ZERO, temp);
		Vector4f result;
		RegisterStoreAligned(temp, &result);
		return Color::FromClampedVector(result);
	}

	// The count of address mode.
	constexpr int ADDRESS_MAX = static_cast<int>(AddressMode::Max);

	/**
		* @brief Calculates function table ofindex nearest-sampler function or biilinear-sampler.
		*/
	constexpr int CalculateEntryIndex(const AddressMode& addressX, const AddressMode& addressY)
	{
		return static_cast<int>(addressX) * ADDRESS_MAX + static_cast<int>(addressY);
	}

	/**
		* @brief The function table that filter mode is equals to nearest.
		*/
	constexpr Sampler Nearest[ADDRESS_MAX * ADDRESS_MAX] =
	{
		/* function                                                 location */
		NearestSampler<AddressMode::Warp>,                           /* 0  */
		NearestSampler<AddressMode::Warp,    AddressMode::Mirror>,   /* 1  */
		NearestSampler<AddressMode::Warp,    AddressMode::Clamp>,    /* 2  */
		NearestSampler<AddressMode::Warp,    AddressMode::Border>,   /* 3  */

		NearestSampler<AddressMode::Mirror,  AddressMode::Warp>,     /* 4  */
		NearestSampler<AddressMode::Mirror>,                         /* 5  */
		NearestSampler<AddressMode::Mirror,  AddressMode::Clamp>,    /* 6  */
		NearestSampler<AddressMode::Mirror,  AddressMode::Border>,   /* 7  */

		NearestSampler<AddressMode::Clamp,   AddressMode::Warp>,     /* 8  */
		NearestSampler<AddressMode::Clamp,   AddressMode::Mirror>,   /* 9  */
		NearestSampler<AddressMode::Clamp>,                          /* 10 */
		NearestSampler<AddressMode::Clamp,   AddressMode::Border>,   /* 11 */

		NearestSampler<AddressMode::Border,  AddressMode::Warp>,     /* 12 */
		NearestSampler<AddressMode::Border,  AddressMode::Mirror>,   /* 13 */
		NearestSampler<AddressMode::Border,  AddressMode::Clamp>,    /* 14 */
		NearestSampler<AddressMode::Border>,                         /* 15 */
	};

	/**
		* @brief The function table that filter mode is equals to biilinear.
		*/
	constexpr Sampler Biilinear[ADDRESS_MAX * ADDRESS_MAX] =
	{
		/* function                                                  location */
		BilinearSampler<AddressMode::Warp>,                           /* 0  */
		BilinearSampler<AddressMode::Warp,    AddressMode::Mirror>,   /* 1  */
		BilinearSampler<AddressMode::Warp,    AddressMode::Clamp>,    /* 2  */
		BilinearSampler<AddressMode::Warp,    AddressMode::Border>,   /* 3  */

		BilinearSampler<AddressMode::Mirror,  AddressMode::Warp>,     /* 4  */
		BilinearSampler<AddressMode::Mirror>,                         /* 5  */
		BilinearSampler<AddressMode::Mirror,  AddressMode::Clamp>,    /* 6  */
		BilinearSampler<AddressMode::Mirror,  AddressMode::Border>,   /* 7  */

		BilinearSampler<AddressMode::Clamp,   AddressMode::Warp>,     /* 8  */
		BilinearSampler<AddressMode::Clamp,   AddressMode::Mirror>,   /* 9  */
		BilinearSampler<AddressMode::Clamp>,                          /* 10 */
		BilinearSampler<AddressMode::Clamp,   AddressMode::Border>,   /* 11 */

		BilinearSampler<AddressMode::Border,  AddressMode::Warp>,     /* 12 */
		BilinearSampler<AddressMode::Border,  AddressMode::Mirror>,   /* 13 */
		BilinearSampler<AddressMode::Border,  AddressMode::Clamp>,    /* 14 */
		BilinearSampler<AddressMode::Border>,                         /* 15 */
	};



	/**
	 * @brief Texture sampler functor.
	 */
	struct TextureSampler
	{
		const ATexture* target;

		Sampler sampler;

	public:
		TextureSampler() = delete;
		
		~TextureSampler() = default;

		explicit TextureSampler(const ATexture* inTarget) : target(inTarget)
		{
			if (inTarget)
			{
				int entry = CalculateEntryIndex(inTarget->xAddressMode, inTarget->yAddressMode);
				if (ATexture::FilterMode::Nearest == inTarget->filterMode)
				{
					sampler = Nearest[entry];
				}
				else if (ATexture::FilterMode::Bilinear == inTarget->filterMode)
				{
					sampler = Biilinear[entry];
				}
				else
				{
					sampler = Nearest[entry];
				}
			}
			else
			{
				sampler = nullptr;
			}
		}

		force_inline Color operator() (const Vector2f& uv) const noexcept { return sampler(target, uv.x, uv.y); }
		
		force_inline Color operator() (const float& u, const float& v) const noexcept { return sampler(target, u, v); }
	};
}