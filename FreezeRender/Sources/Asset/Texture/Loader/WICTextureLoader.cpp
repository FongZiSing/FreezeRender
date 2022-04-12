#include "WICTextureLoader.hpp"
#include <Asset/Utility/LoaderUitlity.hpp>

// Windows header.
#include <wincodec.h>

// Smart Pointer Header.
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;



/**
 * @brief TODO Faster loader with windows imaging component.
*/
namespace LoaderDetail
{
	Array<ExtesionDescriptor> targets = {
		{ L".jpeg",  (unsigned)WICTextureLoader::Extension::JPEG },
		{ L".jpg",   (unsigned)WICTextureLoader::Extension::JPG  },
		{ L".bmp",   (unsigned)WICTextureLoader::Extension::BMP  },
		{ L".png",   (unsigned)WICTextureLoader::Extension::PNG  },
	};

	const GUID pGUID[APixelFormat::Max] =
	{
		GUID_WICPixelFormatUndefined,
		GUID_WICPixelFormat32bppPRGBA,
		GUID_WICPixelFormat128bppPRGBAFloat,
	};

	WICTextureLoader::Status Load(const wchar_t* const filepath, Texture* result, APixelFormat asformat)
	{
		using Status = WICTextureLoader::Status;

		if (pGUID[asformat] == GUID_WICPixelFormatUndefined)
		{
			return Status::FormatNotSupported;
		}

		HRESULT hr = S_OK;

		// Create WIC factory.
		ComPtr<IWICImagingFactory> pIWICFactory;
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(pIWICFactory.GetAddressOf())
		);
		if (!SUCCEEDED(hr))
		{
			return Status::WICStartupFailed;
		}

		// Create a decoder.
		ComPtr<IWICBitmapDecoder> pDecoder;
		hr = pIWICFactory->CreateDecoderFromFilename(
			filepath,                        // Image to be decoded.
			NULL,                            // Do not prefer a particular vendor.
			GENERIC_READ,                    // Desired read access to the file.
			WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed.
			pDecoder.GetAddressOf()          // Pointer to the decoder.
		);
		if (!SUCCEEDED(hr))
		{
			return WICTextureLoader::Status::WICStartupFailed;
		}

		// Create converter.
		ComPtr<IWICFormatConverter> pConvertedSourceBitmap;
		hr = pIWICFactory->CreateFormatConverter(&pConvertedSourceBitmap);
		if (!SUCCEEDED(hr))
		{
			return WICTextureLoader::Status::WICStartupFailed;
		}

		// Retrieve the first frame of the image from the decoder.
		ComPtr<IWICBitmapFrameDecode> pFrame;
		hr = pDecoder->GetFrame(0, pFrame.GetAddressOf());
		if (!SUCCEEDED(hr))
		{
			return WICTextureLoader::Status::ImageParseFailed;
		}

		// Format convert the frame to 32bppPRGBA
		hr = pConvertedSourceBitmap->Initialize(
			pFrame.Get(),                    // Input bitmap to convert.
			pGUID[asformat],                 // Destination pixel format.
			WICBitmapDitherTypeNone,         // Specified dither pattern.
			NULL,                            // Specify a particular palette.
			0.f,                             // Alpha threshold.
			WICBitmapPaletteTypeCustom       // Palette translation type.
		);
		if (!SUCCEEDED(hr))
		{
			return WICTextureLoader::Status::ImageParseFailed;
		}

		// Get image size.
		unsigned int width = 0, height = 0;
		hr = pConvertedSourceBitmap->GetSize(&width, &height);
		if (!SUCCEEDED(hr) || width >= TEXTURE_MAX_WIDTH || height >= TEXTURE_MAX_HEIGHT)
		{
			return WICTextureLoader::Status::ImageParseFailed;
		}

		// Copy source data.
		WICRect rect = { 0, 0, static_cast<int>(width), static_cast<int>(height) };
		unsigned int strides = width * GPixelFormat[asformat].bytePerPixel;
		unsigned int blocks = height * strides;
		Bulkdata<unsigned char> buffer;
		buffer.Reallocate(strides * height);
		hr = pConvertedSourceBitmap->CopyPixels(&rect, strides, blocks, buffer.Get());
		if (!SUCCEEDED(hr))
		{
			return WICTextureLoader::Status::ImageParseFailed;
		}

		// Initialize texture.
		result->format = asformat;
		result->bytePerChannels = GPixelFormat[asformat].bytePerChannels;
		result->bytePerPixel = GPixelFormat[asformat].bytePerPixel;
		result->channels = GPixelFormat[asformat].channelCount;
		result->strides = static_cast<int>(strides);
		result->width = static_cast<int>(width);
		result->height = static_cast<int>(height);
		result->bits.Swap(buffer);
		return Status::Ok;
	}
}


WICTextureLoader::WICTextureLoader(const std::filesystem::path& filepath)
{
	this->filepath = filepath;
	int index = IsExtesionEqual(this->filepath, LoaderDetail::targets);
	this->extension = index >= 0 ? (Extension)LoaderDetail::targets[index].reserved1 : Extension::Unknown;
	this->status = Verify();
}

WICTextureLoader::WICTextureLoader(std::filesystem::path&& filepath)
{
	this->filepath = std::move(filepath);
	int index = IsExtesionEqual(this->filepath, LoaderDetail::targets);
	this->extension = index >= 0 ? (Extension)LoaderDetail::targets[index].reserved1 : Extension::Unknown;
	this->status = Verify();
}

WICTextureLoader::Status WICTextureLoader::Verify() const
{
	if (filepath.empty())
	{
		return Status::initFailed;
	}

	if (!std::filesystem::exists(filepath))
	{
		return Status::initFailed;
	}

	if (extension == Extension::Unknown)
	{
		return Status::initFailed;
	}
	return Status::initSuccess;
}

WICTextureLoader::Status WICTextureLoader::Load(Texture* result, APixelFormat asformat)
{
	if (status == Status::initFailed)
	{
		return status;
	}

	if (result == nullptr || asformat <= APixelFormat::None || asformat >= APixelFormat::Max)
	{
		status = Status::InvalidInput;
		return status;
	}

	Status status = LoaderDetail::Load(this->GetAbsolutePath().Data(), result, asformat);
	if (status != Status::Ok)
	{
		result->format = APixelFormat::None;
		result->bytePerChannels = 0;
		result->bytePerPixel = 0;
		result->channels = 0;
		result->strides = 0;
		result->width = 0;
		result->height = 0;
		result->bits.Deallocate();
		result->name.Clear();
		result->id.Clear();
	}
	else
	{
		result->name = filepath.filename();
		static std::atomic<long long> id = 0;
		result->id = WideString(L"WICLoaderMesh_").Append(GenerateId(id));
	}

	return status;
}
