//
// WICTextureLoader.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Utility of load texture by using WIC.
//
#pragma once

#include "TextureLoader.hpp"
#include <filesystem>



namespace Pluto
{
	class WICTextureLoader final : public TextureLoader
	{
		// Store the last result of operation.
		Status status = Status::Uninitialized;

		// Store the image file path. 
		std::filesystem::path filepath;

		// Store the supported file extension.
		Extension extension;

	public:

		//--------------------------------
		//~ Begin initialize.
		WICTextureLoader(const std::filesystem::path& filepath);

		WICTextureLoader(std::filesystem::path&& filepath);

		WICTextureLoader(const wchar_t* const filepath) : WICTextureLoader(std::filesystem::path(filepath)) {}
		
		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin inline function.

		WideString GetNativeName() const override { return filepath.native(); }

		WideString GetAbsolutePath() const override { return std::filesystem::canonical(filepath).wstring(); }

		WideString GetName() const override { return filepath.filename().wstring(); }

		Extension GetExtension() const override { return extension; }
		
		//~ Inline function.
		//--------------------------------


		Status Verify() const override;

		Status Load(ATexture* result, APixelFormat asformat = APixelFormat::FLOAT_RGBA) override;
	};
}