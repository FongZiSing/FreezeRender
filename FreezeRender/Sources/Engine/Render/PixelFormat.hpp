//
// PixelFormat.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of pixel format.
//
#pragma once



namespace Pluto
{
	/**
	 * @brief Pixel format of a texture, not of a render target.
	 */
	enum APixelFormat : unsigned char
	{
		None = 0,
		UCHAR_RGBA = 1,
		FLOAT_RGBA = 2,
		Max,
	};



	/**
	 * @brief Detail pixel format information of a texture, not of a render target.
	 */
	struct PixelFormatInfo
	{
		const wchar_t* const name;
		const wchar_t* const Info;
		bool bSupported;
		unsigned char bytePerChannels;
		unsigned char bytePerPixel;
		unsigned char channelCount;
	};



	/**
	 * @brief Global constant of detail pixel format information.
	 */
	static const PixelFormatInfo GPixelFormat[APixelFormat::Max] =
	{
		/* Name            Info                                          Support    Byte per channels    Byte per pixel    Count of channels */
		{  L"UnKnown",     L"Unknown format",                             false,           0,                  0,                 0           },
		{  L"UCHAR_RGBA",  L"RGBA, unsigned byte (8 bits) per channels",  true,            1,                  4,                 4           },
		{  L"FLOAT_RGBA",  L"RGBA, float (32 bits) per channels",         false,           4,                 16,                 4           },
	};
}