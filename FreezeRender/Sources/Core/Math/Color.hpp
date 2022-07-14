//
// Math/Color.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Definitions of color.
//
#pragma once

#include <Common.hpp>
#include "Matrix.hpp"



namespace Pluto
{
	/**
	 * @brief The 32-bit RGBA color.
	 */
	struct Color
	{
		union
		{
			struct { unsigned char r, g, b, a; };
			unsigned int value;
		};


		//--------------------------------
		//~ Begin Constants.

		static const Color Transparent;
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;

		//~ End Constants.
		//--------------------------------


		//--------------------------------
		//~ Begin initialize.

		Color() noexcept : b(0), g(0), r(0), a(255) {}
		Color(const unsigned char& inR, const unsigned char& inG, const unsigned char& inB, const unsigned char& inA = 255) noexcept : r(inR), g(inG), b(inB), a(inA) {}
		warn_nodiscard inline static Color FromLinearVector(const float& linearR, const float& linearG, const float& linearB, const float& linearA);
		warn_nodiscard inline static Color FromLinearVector(const float& linearR, const float& linearG, const float& linearB);
		warn_nodiscard force_inline static Color FromLinearVector(const Vector4& linearColor);
		warn_nodiscard force_inline static Color FromLinearVector(const Vector3& linearColor);
		warn_nodiscard inline static Color FromClampedVector(const Vector4& color);
		warn_nodiscard inline static Color FromClampedVector(const Vector3& color);
		warn_nodiscard inline static Color FromVector(const Vector4& color);
		warn_nodiscard inline static Color FromVector(const Vector3& color);
	
		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin self-related operations.
	
		bool operator == (const Color& rhs) { return value == rhs.value; }
		bool operator != (const Color& rhs) { return value != rhs.value; }
	
		//~ End self-related operations.
		//--------------------------------
	};



#pragma region contants

	const inline_variable Color Color::Transparent = { 0u, 0u, 0u, 0u };
	const inline_variable Color Color::Black       = { 0u, 0u, 0u, 255u };
	const inline_variable Color Color::White       = { 255u, 255u, 255u, 255u };
	const inline_variable Color Color::Red         = { 255u, 0u, 0u, 255u };
	const inline_variable Color Color::Green       = { 0u, 255u, 0u, 255u };
	const inline_variable Color Color::Blue        = { 0u, 0u, 255u, 255u };

#pragma endregion contants



#pragma region implemention

	inline Color Color::FromLinearVector(const float& linearR, const float& linearG, const float& linearB, const float& linearA)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, linearR * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearG * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearB * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearA * 255, 255.f))
		};
	}

	inline Color Color::FromLinearVector(const float& linearR, const float& linearG, const float& linearB)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, linearR * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearG * 255, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, linearB * 255, 255.f)),
			255
		};
	}

	force_inline Color Color::FromLinearVector(const Vector4& linearColor)
	{
		return FromLinearVector(linearColor.x, linearColor.y, linearColor.z, linearColor.w);
	}

	force_inline Color Color::FromLinearVector(const Vector3& linearColor)
	{
		return FromLinearVector(linearColor.x, linearColor.y, linearColor.z);
	}

	inline Color Color::FromClampedVector(const Vector4& color)
	{
		return
		{
			static_cast<unsigned char>(color.x),
			static_cast<unsigned char>(color.y),
			static_cast<unsigned char>(color.z),
			static_cast<unsigned char>(color.w)
		};
	}

	inline Color Color::FromClampedVector(const Vector3& color)
	{
		return
		{
			static_cast<unsigned char>(color.x),
			static_cast<unsigned char>(color.y),
			static_cast<unsigned char>(color.z)
		};
	}

	inline Color Color::FromVector(const Vector4& color)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, color.x, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.y, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.z, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.w, 255.f))
		};
	}

	inline Color Color::FromVector(const Vector3& color)
	{
		return
		{
			static_cast<unsigned char>(Math::Clamp(0.f, color.x, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.y, 255.f)),
			static_cast<unsigned char>(Math::Clamp(0.f, color.z, 255.f))
		};
	}

#pragma endregion implemention
}