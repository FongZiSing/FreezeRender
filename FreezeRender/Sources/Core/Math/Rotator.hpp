//
// Math/Rotator.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
//  Euler angles definition.
//
#pragma once

#include <Common.hpp>
#include "Matrix.hpp"



namespace Pluto
{
	/**
	 * @brief Implements a container for rotation information.
	 *        all rotation values are stored in degrees, range in [-180, 180].
	 *        the rotation order is Y-X-Z
	 * @note
	 *    Y (yaw operation)
	 *             |  / -Z (roll operation)
	 *             | /
	 *             |/______ X (pitch operation)
	 */
	struct Rotator
	{
		// Rotation around the up axis (around Y-Axis).
		float yaw;

		// Rotation around the right axis (around X-Axis).
		float pitch;

		// Rotation around the forward axis (around negative Z-Axis).
		float roll;


		//--------------------------------
		//~ Begin constants.

		static const Rotator Zero;

		//~ End constants.
		//--------------------------------


		//--------------------------------
		//~ Begin initialize.

		Rotator() noexcept : yaw(0.f), pitch(0.f), roll(0.f) {}
		Rotator(const float& inYaw, const float& inPitch = 0.f, const float& inRoll = 0.f) noexcept : yaw(inYaw), pitch(inPitch), roll(inRoll) {}
		Rotator(const Vector2f& inValue, const float& inRoll = 0.f) : yaw(inValue.x), pitch(inValue.y), roll(inRoll) {}
		Rotator(const float& inYaw, const Vector2f& in) : yaw(inYaw), pitch(in.x), roll(in.y) {}
		Rotator(const Rotator& inValue) : yaw(inValue.yaw), pitch(inValue.pitch), roll(inValue.roll) {}
		Rotator(const Vector3f& euler) : yaw(euler.z), pitch(euler.y), roll(euler.x) {}
		force_inline Rotator& operator = (const Rotator& inValue);

		//~ End initialize.
		//--------------------------------


		//--------------------------------
		//~ Begin self-related operations.

		force_inline Vector3f Euler();
		force_inline bool operator == (const Rotator& rhs);
		force_inline bool operator != (const Rotator& rhs);
		force_inline Rotator operator + (const Rotator& rhs);
		force_inline Rotator operator - (const Rotator& rhs);
		force_inline Rotator operator * (const Rotator& rhs);
		force_inline Rotator operator / (const Rotator& rhs);
		force_inline Rotator operator + (const Rotator& rhs) const;
		force_inline Rotator operator - (const Rotator& rhs) const;
		force_inline Rotator operator * (const Rotator& rhs) const;
		force_inline Rotator operator / (const Rotator& rhs) const;
		force_inline const Rotator& operator += (const Rotator& rhs);
		force_inline const Rotator& operator -= (const Rotator& rhs);
		force_inline const Rotator& operator *= (const Rotator& rhs);
		force_inline const Rotator& operator /= (const Rotator& rhs);

		//~ End self-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin number-related operations.

		force_inline Rotator operator + (const float& rhs);
		force_inline Rotator operator - (const float& rhs);
		force_inline Rotator operator * (const float& rhs);
		force_inline Rotator operator / (const float& rhs);
		force_inline Rotator operator + (const float& rhs) const;
		force_inline Rotator operator - (const float& rhs) const;
		force_inline Rotator operator * (const float& rhs) const;
		force_inline Rotator operator / (const float& rhs) const;
		force_inline const Rotator& operator += (const float& rhs);
		force_inline const Rotator& operator -= (const float& rhs);
		force_inline const Rotator& operator *= (const float& rhs);
		force_inline const Rotator& operator /= (const float& rhs);

		//~ End number-related operations.
		//--------------------------------


		//--------------------------------
		//~ Begin rotator operations.

		warn_nodiscard force_inline Rotator Normalize() const;
		force_inline void Normalized();

		warn_nodiscard force_inline Matrix44f ToMatrix() const;
		warn_nodiscard force_inline Matrix44f ToInvMatrix() const;

		warn_nodiscard force_inline Vector3f GetForwardVector() const; // the same as `ToMatrix() * Vector3f( 0, 0, -1 )`.
		warn_nodiscard force_inline Vector3f GetRightVector() const;   // the same as `ToMatrix() * Vector3f( 1, 0,  0 )`.
		warn_nodiscard force_inline Vector3f GetUpVector() const;      // the same as `ToMatrix() * Vector3f( 0, 1,  0 )`

		//~ End rotator operations.
		//--------------------------------
	};


#pragma region implemention

	const inline_variable Rotator Rotator::Zero = { 0.f };



	force_inline Rotator& Rotator::operator = (const Rotator& inValue) { yaw = inValue.yaw; pitch = inValue.pitch; roll = inValue.roll; return *this; }

	force_inline Vector3f Rotator::Euler() { return Vector3f(roll, pitch, yaw); }
	force_inline bool Rotator::operator == (const Rotator& rhs) { return rhs.yaw == yaw && rhs.pitch == pitch && rhs.roll == roll; }
	force_inline bool Rotator::operator != (const Rotator& rhs) { return rhs.yaw != yaw || rhs.pitch != pitch || rhs.roll != roll; }
	force_inline Rotator Rotator::operator + (const Rotator& rhs) { return Rotator(yaw + rhs.yaw, pitch + rhs.pitch, roll + rhs.roll); }
	force_inline Rotator Rotator::operator - (const Rotator& rhs) { return Rotator(yaw - rhs.yaw, pitch - rhs.pitch, roll - rhs.roll); }
	force_inline Rotator Rotator::operator * (const Rotator& rhs) { return Rotator(yaw * rhs.yaw, pitch * rhs.pitch, roll * rhs.roll); }
	force_inline Rotator Rotator::operator / (const Rotator& rhs) { return Rotator(yaw / rhs.yaw, pitch / rhs.pitch, roll / rhs.roll); }
	force_inline Rotator Rotator::operator + (const Rotator& rhs) const { return Rotator(yaw + rhs.yaw, pitch + rhs.pitch, roll + rhs.roll); }
	force_inline Rotator Rotator::operator - (const Rotator& rhs) const { return Rotator(yaw - rhs.yaw, pitch - rhs.pitch, roll - rhs.roll); }
	force_inline Rotator Rotator::operator * (const Rotator& rhs) const { return Rotator(yaw * rhs.yaw, pitch * rhs.pitch, roll * rhs.roll); }
	force_inline Rotator Rotator::operator / (const Rotator& rhs) const { return Rotator(yaw / rhs.yaw, pitch / rhs.pitch, roll / rhs.roll); }
	force_inline const Rotator& Rotator::operator += (const Rotator& rhs) { yaw += rhs.yaw; pitch += rhs.pitch; roll += rhs.roll; return *this; }
	force_inline const Rotator& Rotator::operator -= (const Rotator& rhs) { yaw -= rhs.yaw; pitch -= rhs.pitch; roll -= rhs.roll; return *this; }
	force_inline const Rotator& Rotator::operator *= (const Rotator& rhs) { yaw *= rhs.yaw; pitch *= rhs.pitch; roll *= rhs.roll; return *this; }
	force_inline const Rotator& Rotator::operator /= (const Rotator& rhs) { yaw /= rhs.yaw; pitch /= rhs.pitch; roll /= rhs.roll; return *this; }

	force_inline Rotator Rotator::operator + (const float& rhs) { return Rotator(yaw + rhs, pitch + rhs, roll + rhs); }
	force_inline Rotator Rotator::operator - (const float& rhs) { return Rotator(yaw - rhs, pitch - rhs, roll - rhs); }
	force_inline Rotator Rotator::operator * (const float& rhs) { return Rotator(yaw * rhs, pitch * rhs, roll * rhs); }
	force_inline Rotator Rotator::operator / (const float& rhs) { return Rotator(yaw / rhs, pitch / rhs, roll / rhs); }
	force_inline Rotator Rotator::operator + (const float& rhs) const { return Rotator(yaw + rhs, pitch + rhs, roll + rhs); }
	force_inline Rotator Rotator::operator - (const float& rhs) const { return Rotator(yaw - rhs, pitch - rhs, roll - rhs); }
	force_inline Rotator Rotator::operator * (const float& rhs) const { return Rotator(yaw * rhs, pitch * rhs, roll * rhs); }
	force_inline Rotator Rotator::operator / (const float& rhs) const { return Rotator(yaw / rhs, pitch / rhs, roll / rhs); }
	force_inline const Rotator& Rotator::operator += (const float& rhs) { yaw += rhs; pitch += rhs; roll += rhs; return *this; }
	force_inline const Rotator& Rotator::operator -= (const float& rhs) { yaw -= rhs; pitch -= rhs; roll -= rhs; return *this; }
	force_inline const Rotator& Rotator::operator *= (const float& rhs) { yaw *= rhs; pitch *= rhs; roll *= rhs; return *this; }
	force_inline const Rotator& Rotator::operator /= (const float& rhs) { yaw /= rhs; pitch /= rhs; roll /= rhs; return *this; }

	force_inline Rotator Rotator::Normalize() const
	{
		Rotator result;
		Math::NormalizeRotator(this, &result);
		return  result;
	}

	force_inline void Rotator::Normalized()
	{
		Math::NormalizeRotator(this, this);
	}

	force_inline Matrix44f Rotator::ToMatrix() const
	{
		// NOTE: intrinsic rotation.
		Matrix44f result;
		Vector4f radians;
		Vector4f degrees{ 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.m[0][0] = c[1] * c[3] + s[1] * s[2] * s[3];
		result.m[0][1] = s[1] * s[2] * c[3] - c[1] * s[3];
		result.m[0][2] = s[1] * c[2];
		result.m[0][3] = 0;

		result.m[1][0] = c[2] * s[3];
		result.m[1][1] = c[2] * c[3];
		result.m[1][2] = -s[2];
		result.m[1][3] = 0;

		result.m[2][0] = c[1] * s[2] * s[3] - s[1] * c[3];
		result.m[2][1] = c[1] * s[2] * c[3] + s[1] * s[3];
		result.m[2][2] = c[1] * c[2];
		result.m[2][3] = 0;

		result.m[3][0] = 0;
		result.m[3][1] = 0;
		result.m[3][2] = 0;
		result.m[3][3] = 1;

		return result;
	}

	force_inline Matrix44f Rotator::ToInvMatrix() const
	{
		Matrix44f result;
		Vector4f radians;
		Vector4f degrees{ 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.m[0][0] = c[1] * c[3] + s[1] * s[2] * s[3];
		result.m[0][1] = c[2] * s[3];
		result.m[0][2] = c[1] * s[2] * s[3] - s[1] * c[3];
		result.m[0][3] = 0;

		result.m[1][0] = s[1] * s[2] * c[3] - c[1] * s[3];
		result.m[1][1] = c[2] * c[3];
		result.m[1][2] = c[1] * s[2] * c[3] + s[1] * s[3];
		result.m[1][3] = 0;

		result.m[2][0] = s[1] * c[2];
		result.m[2][1] = -s[2];
		result.m[2][2] = c[1] * c[2];
		result.m[2][3] = 0;

		result.m[3][0] = 0;
		result.m[3][1] = 0;
		result.m[3][2] = 0;
		result.m[3][3] = 1;

		return result;
	}

	force_inline Vector3f Rotator::GetForwardVector() const
	{
		Vector3f result;
		Vector4f radians;
		Vector4f degrees{ 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.x = -s[1] * c[2];
		result.y = s[2];
		result.z = -c[1] * c[2];

		return result;
	}

	force_inline Vector3f Rotator::GetRightVector() const
	{
		Vector3f result;
		Vector4f radians;
		Vector4f degrees{ 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.x = c[1] * c[3] + s[1] * s[2] * s[3];
		result.y = c[2] * s[3];
		result.z = c[1] * s[2] * s[3] - s[1] * c[3];

		return result;
	}

	force_inline Vector3f Rotator::GetUpVector() const
	{
		Vector3f result;
		Vector4f radians;
		Vector4f degrees{ 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.x = s[1] * s[2] * c[3] - c[1] * s[3];
		result.y = c[2] * c[3];
		result.z = c[1] * s[2] * c[3] + s[1] * s[3];

		return result;
	}

#pragma endregion implemention.
}