#pragma once

#include "TextureLoader.hpp"
#include <filesystem>



class WICTextureLoader final : public TextureLoader
{
	// Store the last result of operation.
	Status status = Status::Uninitialized;

	// Store the image file path. 
	std::filesystem::path filepath;

	// Store the supported file extension.
	Extension extension;

public:
	/// Initialize.
	WICTextureLoader(const std::filesystem::path& filepath);

	WICTextureLoader(std::filesystem::path&& filepath);
	
	WICTextureLoader(const wchar_t* const filepath) : WICTextureLoader(std::filesystem::path(filepath)) {}
	/// Initialize.

	/// Inline function.
	WideString GetNativeName() const override { return filepath.native(); }

	WideString GetAbsolutePath() const override { return std::filesystem::canonical(filepath).wstring(); }

	WideString GetName() const override { return filepath.filename().wstring(); }
	
	Extension GetExtension() const override { return extension; }
	/// Inline function.

	Status Verify() const override;

	Status Load(Texture* result, APixelFormat asformat = APixelFormat::FLOAT_RGBA) override;
};