#pragma once

#include <Common.hpp>
#include <Container/String.hpp>
#include <Core/Texture.hpp>



struct interface_as TextureLoader
{
	/**
	 * @brief Specifies status of loading a image file.
	 */
	enum class Status : unsigned char
	{
		Ok = 0,
		Uninitialized,
		initFailed,
		initSuccess,
		InvalidInput,
		FileNotExisted,
		FileOpenFailed,
		ImageParseFailed,
		OutOfMemory,

		FormatNotSupported = 128,
		WICStartupFailed,
	};

	/**
	 * @brief Specifies supported image file.
	 */
	enum class Extension : unsigned char
	{
		Unknown = 0,
		JPEG,
		JPG,
		BMP,
		PNG,
		//TIFF,
	};

	/**
	 * @brief Determines whether the loading is successful.
	 */
	friend constexpr bool Success(Status status)
	{
		return status == Status::Ok;
	}

	/**
	 * @brief Returns the native path of the specified image file.
	 */
	virtual WideString GetNativeName() const = 0;

	/**
	 * @brief Returns the absolute path of the specified image file.
	 */
	virtual WideString GetAbsolutePath() const = 0;

	/**
	 * @brief Returns the name of the specified image file.
	 */
	virtual WideString GetName() const = 0;

	/**
	 * @brief Returns the type of image.
	*/
	virtual Extension GetExtension() const = 0;

	/**
	 * @brief Determines whether the specified image file has a valid extension or exists, or everything you like.
	 *        It should be defined as pure function (Referential Transparency Principle) if inherit this class.
	 */
	virtual Status Verify() const = 0;

	/**
	 * @brief Try to load the specified image file.
	 */
	virtual Status Load(Texture* result, APixelFormat asformat = APixelFormat::FLOAT_RGBA) = 0;
};
