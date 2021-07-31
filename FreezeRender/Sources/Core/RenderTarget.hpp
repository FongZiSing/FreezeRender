#pragma once

#include <Common.hpp>
#include <Container/Bulkdata.hpp>
#include <Utility/Math.hpp>



/**
 * @brief The image can be rendered on screen devices.
 */
template <typename PixelType, PixelType defaultValue>
class RenderTarget
{
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
	force_inline const PixelType& GetPixel(const int& index) const;
};



/**
 * @brief The render target for the final ouput.
 */
typedef RenderTarget<unsigned int, 0> SceneRenderTarget;



/**
 * @brief The render target for the z-depth.
 */
typedef RenderTarget<float, -std::numeric_limits <float>::infinity()> DepthRenderTarget;



/**
 * @brief The Detail implemention of `RenderTarget` template class.
 */
#ifndef RENDERTARGET_HPP_RENDERTARGET_IMPL
#define RENDERTARGET_HPP_RENDERTARGET_IMPL

	template <typename PixelType, PixelType defaultValue>
	RenderTarget<PixelType, defaultValue>::RenderTarget(const int& inWidth, const int& inHeight)
	{
		Resize(inWidth, inHeight);
	}

	template <typename PixelType, PixelType defaultValue>
	void RenderTarget<PixelType, defaultValue>::Resize(const int& inWidth, const int& inHeight)
	{
		if (inWidth > 0 && inHeight > 0)
		{
			width = inWidth;
			height = inHeight;
			data.Reallocate(inWidth * inHeight);
			data.Initialize(defaultValue);
		}
	}

	template <typename PixelType, PixelType defaultValue>
	force_inline bool RenderTarget<PixelType, defaultValue>::IsValid() const
	{
		return data.Size() != 0 && width > 0 && height > 0;
	}

	template <typename PixelType, PixelType defaultValue>
	force_inline bool RenderTarget<PixelType, defaultValue>::IsValid(const int& index)
	{
		return data.Size() > index;
	}

	template <typename PixelType, PixelType defaultValue>
	force_inline bool RenderTarget<PixelType, defaultValue>::IsValid(const int& inWidth, const int& inHeight)
	{
		return Math::IsWithin(0, inWidth, width) && Math::IsWithin(0, inHeight, height);
	}

	template <typename PixelType, PixelType defaultValue>
	force_inline unsigned char* RenderTarget<PixelType, defaultValue>::Data()
	{
		return (unsigned char*)data.Get();
	}

	template <typename PixelType, PixelType defaultValue>
	inline void RenderTarget<PixelType, defaultValue>::Clear()
	{
		data.Initialize(defaultValue);
	}

	template <typename PixelType, PixelType defaultValue>
	template<typename OtherPixelType> void RenderTarget<PixelType, defaultValue>::SetPixel(const int& index, const OtherPixelType& value)
	{
		data[index] = *(const PixelType*)(&value);
	}

	template <typename PixelType, PixelType defaultValue>
	force_inline void RenderTarget<PixelType, defaultValue>::SetPixel(const int& index, const PixelType& value)
	{
		data[index] = value;
	}

	template <typename PixelType, PixelType defaultValue>
	force_inline const PixelType& RenderTarget<PixelType, defaultValue>::GetPixel(const int& index) const
	{
		return data[index];
	}

#endif // !RENDERTARGET_HPP_RENDERTARGET_IMPL