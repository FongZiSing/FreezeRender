#pragma once

#include <Common.hpp>
#include <Core/Color.hpp>

/// forward declaration.
struct Texture;
/// forward declaration.



class TextureSampler
{
public:
	/**
	 * @brief Specifies filtering options during texture sampling.
	 */
	enum class FilterMode : unsigned char
	{
		Nearest = 0,
		Bilinear = 1,
		Max // placeholder
		//Trilinear,
	};

	/**
	 * @brief Specifies the way for resolving texture coordinates that are outside of the boundaries of a texture.
	 */
	enum class AddressMode : unsigned char
	{
		Warp = 0,
		Mirror = 1,
		Clamp = 2,
		Border = 3,
		Max // placeholder
	};

	/**
	 * @brief Definition of sampler function entry.
	 */
	using Sampler = Color(*)(const Texture* target, const float& u, const float& v);

	explicit TextureSampler(const Texture* inTarget, FilterMode&& inFilterMode, AddressMode&& inAddressMode);

	explicit TextureSampler(const Texture* inTarget, FilterMode&& inFilterMode, AddressMode&& inAddressModeX, AddressMode&& inAddressModeY);

	force_inline Color operator()(const float& u, const float& v) const { return sampler(target, u, v); }

private:
	const Texture* target;

	FilterMode filterMode;

	AddressMode addressModeX;

	AddressMode addressModeY;

	Sampler sampler;
};
