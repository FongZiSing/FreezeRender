#pragma once

#include <Common.hpp>
#include <Container/Bulkdata.hpp>
#include <Container/String.hpp>
#include "TextureSampler.hpp"
#include "PixelFormat.hpp"
#include "Matrix.hpp"
#include "Color.hpp"
#include <memory>



constexpr unsigned int TEXTURE_MAX_WIDTH = 16 * 1024;  // limit of cols.
constexpr unsigned int TEXTURE_MAX_HEIGHT = 16 * 1024; // limit of rows.


/**
 * @brief Texture object.
 */
struct Texture
{
	// Texture format.
	APixelFormat format = APixelFormat::None;

	// The bytes of one channel.
	unsigned char bytePerChannels = 0;

	// The bytes of one pixel.
	unsigned char bytePerPixel = 0;

	// The numbers of image channel.
	unsigned char channels = 0;

	// The raw data.
	Bulkdata<unsigned char> bits;

	// The batch size for one columns, equals `width * bytePerPixel`.
	int strides = 0;

	// The columns number of image.
	int width = 0;

	// The rows number of image.
	int height = 0;

	// The specified border color, used for `AddressMode::Clamp`.
	Color border = Color::White;

	// The unique id using for scene management.
	WideString id;

	// The texture name using for scene management.
	WideString name;


	explicit Texture() : sampler(this, TextureSampler::FilterMode::Bilinear, TextureSampler::AddressMode::Warp, TextureSampler::AddressMode::Warp) {}

	~Texture() {}

	/// Non-copyable. 
	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;
	/// Non-copyable.

	/**
	 * @brief Query the color with given texture coordinate.
	 * @param uv    - texture coordinate.
	 */
	Color Sample(const Vector2& uv) const { return sampler(uv.x, uv.y); }

private:
	// The unique sampler for this texture.
	const TextureSampler sampler;
};
