//
// RenderTarget.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of render target.
//
#pragma once

#include <Common.hpp>
#include <Container/Bulkdata.hpp>
#include <Math/Matrix.hpp>
#include <Math/Color.hpp>
#include <Math/Math.hpp>



namespace Pluto
{
	/**
	 * @brief The image can be rendered on screen devices.
	 */
	template <typename PixelTraits>
	class RenderTarget
	{
	public:
		using PixelType = PixelTraits::Type;

	protected:
		Bulkdata<PixelType> data;
		int width = 0;
		int height = 0;

	public:
		explicit constexpr RenderTarget(const int& inWidth, const int& inHeight)
		{
			Resize(inWidth, inHeight);
		}

		void Resize(const int& inWidth, const int& inHeight)
		{
			if (inWidth > 0 && inHeight > 0)
			{
				width = inWidth;
				height = inHeight;
				data.Reallocate(1LL * inWidth * inHeight);
				data.Initialize(PixelTraits::defaultPixelValue);
			}
		}

		constexpr int Width() const noexcept { return width; }
		constexpr int Height() const noexcept { return height; }
		constexpr bool IsValid() const noexcept { return data.Size() != 0 && width > 0 && height > 0; }
		constexpr bool IsValid(const int& index) noexcept { return data.Size() > index && index >= 0; }
		constexpr bool IsValid(const int& inWidth, const int& inHeight) noexcept { return Math::IsWithin(0, inWidth, width) && Math::IsWithin(0, inHeight, height); }
		inline void Clear() { data.Initialize(PixelTraits::defaultPixelValue); }
		constexpr unsigned char* Data() noexcept { return (unsigned char*)data.Get(); }
		constexpr const unsigned char* Data() const noexcept { return (unsigned char*)data.Get(); }
		force_inline void Swap(RenderTarget& rhs) { std::swap(width, rhs.width); std::swap(height, rhs.height); data.Swap(rhs.data); }
		template<typename OtherPixelType> constexpr void SetPixel(const int& index, const OtherPixelType& value) { data[index] = *(const PixelType*)(&value); }
		constexpr void SetPixel(const int& index, const PixelType& value) { data[index] = value; }
		constexpr PixelType& GetPixel(const int& index) { return data[index]; }
		constexpr const PixelType& GetPixel(const int& index) const { return data[index]; }
		constexpr PixelTraits::Type GetDefaultPixel() const { return PixelTraits::defaultPixelValue; }
	};



	struct FloatPixelTraits
	{
		using Type = float;
		inline static const Type defaultPixelValue = Number::FLOAT_NEG_INF;
	}; typedef RenderTarget<FloatPixelTraits> FloatRenderTarget;



	struct PosFloatPixelTraits
	{
		using Type = float;
		inline static const Type defaultPixelValue = Number::FLOAT_INF;
	}; typedef RenderTarget<PosFloatPixelTraits> PosFloatRenderTarget;



	struct Float2PixelTraits
	{
		using Type = Vector2f;
		inline static const Type defaultPixelValue = { Number::FLOAT_NAN, Number::FLOAT_NAN };
	}; typedef RenderTarget<Float2PixelTraits> Float2RenderTarget;



	struct Float4PixelTraits
	{
		using Type = Vector4f;
		inline static const Type defaultPixelValue = {
			Number::FLOAT_NAN,
			Number::FLOAT_NAN,
			Number::FLOAT_NAN,
			Number::FLOAT_NAN
		};
	}; typedef RenderTarget<Float4PixelTraits> Float4RenderTarget;



	struct IntPixelTraits
	{
		using Type = int;
		inline static const Type defaultPixelValue = -1;
	}; typedef RenderTarget<IntPixelTraits> IntRenderTarget;



	struct ColorPixelTraits
	{
		using Type = Color;
		inline static const Type defaultPixelValue = Color::Transparent;
	}; typedef RenderTarget<ColorPixelTraits> ColorRenderTarget;

}