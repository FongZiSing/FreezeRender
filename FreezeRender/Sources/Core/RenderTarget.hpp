#pragma once

#include <Common.hpp>
#include <Container/Bulkdata.hpp>
#include <Utility/Math.hpp>
#include "Shadingon.hpp"
#include "Material.hpp"
#include "Matrix.hpp"



/**
 * @brief The image can be rendered on screen devices.
 */
template <typename PixelTraits>
class RenderTarget
{
public:
	using PixelType = PixelTraits::Type;

protected:
	// The raw data.
	Bulkdata<PixelType> data;

	// The columns number of image.
	int width = 0;

	// The rows number of image.
	int height = 0;

public:
	/// Inline function.
	force_inline int Width() const noexcept { return width; }
	force_inline int Height() const noexcept { return height; }
	/// Inline function.

	explicit RenderTarget(const int& inWidth, const int& inHeight);

	/**
	 * @brief Resizes the image to given size.
	 */
	void Resize(const int& inWidth, const int& inHeight);

	/**
	 * @brief Determines whether image size is valid.
	 */
	warn_nodiscard force_inline bool IsValid() const;

	/**
	 * @brief Determines whether the pixel at given index is valid.
	 */
	warn_nodiscard force_inline bool IsValid(const int& index);

	/**
	 * @brief Determines whether the pixel at given location is valid.
	 */
	warn_nodiscard force_inline bool IsValid(const int& inWidth, const int& inHeight);

	/**
	 * @brief Clear the image with default pixel value.
	 */
	inline void Clear();

	/**
	 * @brief Returns source data pointer of the image.
	 */
	warn_nodiscard force_inline unsigned char* Data();

	/**
	 * @brief Returns source data pointer at the beginning.
	 */
	warn_nodiscard force_inline PixelType* Begin() const { return data.Get(); }

	/**
	 * @brief Returns source data pointer at the end.
	 */
	warn_nodiscard force_inline PixelType* End() const { return data.Get(data.Size() - 1); }

	/**
	 * @brief Swap memory without reallocation. 
	 */
	force_inline void Swap(RenderTarget& rhs) { std::swap(width, rhs.width); std::swap(height, rhs.height); data.Swap(rhs.data); }

	/**
	 * @brief Set pixel value at given index.
	 */
	template<typename OtherPixelType>
	void SetPixel(const int& index, const OtherPixelType& value);

	/**
	 * @brief Set pixel value at given index.
	 */
	force_inline void SetPixel(const int& index, const PixelType& value);

	/**
	 * @brief Get pixel value at given index.
	 */
	force_inline PixelType& GetPixel(const int& index) { return data[index]; }

	/**
	 * @brief Get pixel value at given index.
	 */
	force_inline const PixelType& GetPixel(const int& index) const { return data[index]; }

	/**
	 * @brief Returns default pixel value.
	 */
	constexpr PixelTraits::Type GetDefaultPixel() const { return PixelTraits::defaultPixelValue; }
};



/**
 * @brief The render target for the float[2].
 */
struct Float2PixelTraits
{
	using Type = Vector2;
	inline static const Type defaultPixelValue = { Number::FLOAT_NAN, Number::FLOAT_NAN };
}; typedef RenderTarget<Float2PixelTraits> Float2RenderTarget;



/**
 * @brief The render target for the float[4].
 */
struct Float4PixelTraits
{
	using Type = Vector4;
	inline static const Type defaultPixelValue = {
		Number::FLOAT_NAN,
		Number::FLOAT_NAN,
		Number::FLOAT_NAN,
		Number::FLOAT_NAN
	};
}; typedef RenderTarget<Float4PixelTraits> Float4RenderTarget;



/**
 * @brief The integer render target.
 */
struct IntPixelTraits
{
	using Type = int;
	inline static const Type defaultPixelValue = -1;
}; typedef RenderTarget<IntPixelTraits> IntRenderTarget;



/**
 * @brief The render target for the color.
 */
struct ColorPixelTraits
{
	using Type = Color;
	inline static const Type defaultPixelValue = Color::Transparent;
}; typedef RenderTarget<ColorPixelTraits> ColorRenderTarget;



/**
 * @brief The render target for the z-depth.
 */
struct DepthPixelTraits
{
	using Type = float;
	inline static const Type defaultPixelValue = Number::FLOAT_NEG_INF;
}; typedef RenderTarget<DepthPixelTraits> DepthRenderTarget;



/**
 * @brief The buffer for the shading vertex.
 */
struct ShadingPointBufferPixelTraits
{
	using Type = ShadingPoint;
	inline static float nan = Number::FLOAT_NAN;
	inline static const Type defaultPixelValue = {
		{ nan, nan, nan },
		{ nan, nan, nan },
		{ nan, nan }
	};
}; typedef RenderTarget<ShadingPointBufferPixelTraits> ShadingPointBufferRenderTarget;



/**
 * @brief The buffer for the interpolation.
 */
struct InterpolationBufferDescriptorPixelTraits
{
	using Type = ShadingInterpolation;
	inline static const Type defaultPixelValue = {
		Number::FLOAT_INF,
		Number::FLOAT_INF,
		Number::FLOAT_INF,
		Number::FLOAT_INF
	};
}; typedef RenderTarget<InterpolationBufferDescriptorPixelTraits> InterpolationBufferRenderTarget;



/**
 * @brief The buffer of material.
 */
struct MaterialIdBufferPixelTraits
{
	// Using pointer of material as material id.
	using Type = Material*;
	inline static const Type defaultPixelValue = { nullptr };
}; typedef RenderTarget<MaterialIdBufferPixelTraits> MaterialIdBufferRenderTarget;



/**
 * @brief The Detail implemention of `RenderTarget` template class.
 */
#ifndef RENDERTARGET_HPP_RENDERTARGET_IMPL
#define RENDERTARGET_HPP_RENDERTARGET_IMPL

	template <typename PixelTraits>
	RenderTarget<PixelTraits>::RenderTarget(const int& inWidth, const int& inHeight)
	{
		Resize(inWidth, inHeight);
	}

	template <typename PixelTraits>
	void RenderTarget<PixelTraits>::Resize(const int& inWidth, const int& inHeight)
	{
		if (inWidth > 0 && inHeight > 0)
		{
			width = inWidth;
			height = inHeight;
			data.Reallocate(inWidth * inHeight);
			data.Initialize(PixelTraits::defaultPixelValue);
		}
	}

	template <typename PixelTraits>
	force_inline bool RenderTarget<PixelTraits>::IsValid() const
	{
		return data.Size() != 0 && width > 0 && height > 0;
	}

	template <typename PixelTraits>
	force_inline bool RenderTarget<PixelTraits>::IsValid(const int& index)
	{
		return data.Size() > index;
	}

	template <typename PixelTraits>
	force_inline bool RenderTarget<PixelTraits>::IsValid(const int& inWidth, const int& inHeight)
	{
		return Math::IsWithin(0, inWidth, width) && Math::IsWithin(0, inHeight, height);
	}

	template <typename PixelTraits>
	force_inline unsigned char* RenderTarget<PixelTraits>::Data()
	{
		return (unsigned char*)data.Get();
	}

	template <typename PixelTraits>
	inline void RenderTarget<PixelTraits>::Clear()
	{
		data.Initialize(PixelTraits::defaultPixelValue);
	}

	template <typename PixelTraits>
	template<typename OtherPixelType> void RenderTarget<PixelTraits>::SetPixel(const int& index, const OtherPixelType& value)
	{
		data[index] = *(const PixelType*)(&value);
	}

	template <typename PixelTraits>
	force_inline void RenderTarget<PixelTraits>::SetPixel(const int& index, const PixelType& value)
	{
		data[index] = value;
	}

#endif // !RENDERTARGET_HPP_RENDERTARGET_IMPL