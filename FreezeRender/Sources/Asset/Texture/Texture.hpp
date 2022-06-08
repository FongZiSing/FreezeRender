#pragma once

#include <Common.hpp>
#include <Container/Bulkdata.hpp>
#include <Container/String.hpp>
#include <Engine/Render/RenderPixelFormat.hpp>
#include <Math/Color.hpp>



constexpr unsigned int TEXTURE_MAX_WIDTH = 16 * 1024;  // limit of cols.
constexpr unsigned int TEXTURE_MAX_HEIGHT = 16 * 1024; // limit of rows.



/**
 * @brief Texture object.
 */
struct ATexture
{
	/**
	 * @brief Specifies filtering options during texture sampling.
	 */
	enum class FilterMode : unsigned char
	{
		Nearest  = 0,
		Bilinear = 1,
		Max // placeholder
	};



	/**
	 * @brief Specifies the way for resolving texture coordinates that are outside of the boundaries of a texture.
	 */
	enum class AddressMode : unsigned char
	{
		Warp   = 0,
		Mirror = 1,
		Clamp  = 2,
		Border = 3,
		Max // placeholder
	};



	APixelFormat format                = APixelFormat::None;   // Texture format.
	unsigned char bytePerChannels      = 0;
	unsigned char bytePerPixel         = 0;
	unsigned char channels             = 0;                    // The numbers of image channel.
	Bulkdata<unsigned char> bits;                              // The raw data.
	int strides = 0;                                           // The batch size for one columns, equals `width * bytePerPixel`.
	int width                          = 0;
	int height                         = 0;
	Color border = Color::White;                               // The specified border color, used for `AddressMode::Clamp`.
	WideString id;                                             // The unique id using for scene management.
	WideString name;                                           // The texture name using for scene management.
	FilterMode filterMode              = FilterMode::Bilinear;
	AddressMode xAddressMode           = AddressMode::Warp;    // horizontal direction.
	AddressMode yAddressMode           = AddressMode::Warp;    // vertical direction.
	


	ATexture() = default;
	~ATexture() = default;
	ATexture(const ATexture&) = delete;
	ATexture& operator = (const ATexture&) = delete;
};
