#pragma once

#include <Common.hpp>
#include "Matrix.hpp"



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


	/// Constants.
	static const Color Transparent;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	/// Constants.


	/// Initialize.
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
	/// Initialize.


	/// Self-related operations.
	bool operator == (const Color& rhs) { return value == rhs.value; }
	bool operator != (const Color& rhs) { return value != rhs.value; }
	/// Self-related operations.

};



/// Inline static variables.
const inline_variable Color Color::Transparent = { 0u, 0u, 0u, 0u };
const inline_variable Color Color::Black = { 0u, 0u, 0u, 255u };
const inline_variable Color Color::White = { 255u, 255u, 255u, 255u };
const inline_variable Color Color::Red = { 255u, 0u, 0u, 255u };
const inline_variable Color Color::Green = { 0u, 255u, 0u, 255u };
const inline_variable Color Color::Blue = { 0u, 0u, 255u, 255u };
/// Inline static variables.



/**
 * @brief The Detail implemention of `Color` class.
 */
#include "Color.inl"