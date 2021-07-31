#pragma once

#include <Common.hpp>
#include "Matrix.hpp"



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


	/// Constants.
	static const Rotator Zero;
	/// Constants.


	/// Initialize.
	Rotator() noexcept : yaw(0.f), pitch(0.f), roll(0.f) {}
	Rotator(const float& inYaw, const float& inPitch = 0.f, const float& inRoll = 0.f) noexcept : yaw(inYaw), pitch(inPitch), roll(inRoll) {}
	Rotator(const Vector2& inValue, const float& inRoll = 0.f) : yaw(inValue.x), pitch(inValue.y), roll(inRoll) {}
	Rotator(const float& inYaw, const Vector2& in) : yaw(inYaw), pitch(in.x), roll(in.y) {}
	Rotator(const Rotator& inValue) : yaw(inValue.yaw), pitch(inValue.pitch), roll(inValue.roll) {}
	Rotator(const Vector3& euler) : yaw(euler.z), pitch(euler.y), roll(euler.x) {}
	force_inline Rotator& operator = (const Rotator& inValue);
	/// Initialize.


	/// Self-related operations.
	force_inline Vector3 Euler();
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
	/// Self-related operations.


	/// Number-related operations.
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
	/// Number-related operations.

	/**
	 * @brief Normalization.
	 */
	warn_nodiscard force_inline Rotator Normalize() const;

	/**
	 * @brief Self-normalization. the same as `*this = Normalize();`
	 */
	force_inline void Normalized();

	/**
	 * @biref Convert Euler angles to rotation matrix.
	 * @details considered to using intrinsic rotation, so returns `R(yaw) * R(pitch) * R(roll)` (pre-multiply order).
	 */
	warn_nodiscard force_inline Matrix ToMatrix() const;

	/**
	 * @biref Convert Euler angles to inverse rotation matrix, the same as `ToMatrix().Transpose()`
	 */
	warn_nodiscard force_inline Matrix ToInvMatrix() const;


	/**
	 * @biref Returns forward vector, the same as `ToMatrix() * Vector3(0, 0, -1)`.
	 */
	warn_nodiscard force_inline Vector3 GetForwardVector() const;


	/**
	 * @biref Returns left vector, the same as `ToMatrix() * Vector3(1, 0, 0)`.
	 */
	warn_nodiscard force_inline Vector3 GetRightVector() const;


	/**
	 * @biref Returns up vector, the same as `ToMatrix() * Vector3(0, 1, 0)`.
	 */
	warn_nodiscard force_inline Vector3 GetUpVector() const;
};



/// Inline static variables.
const inline_variable Rotator Rotator::Zero = { 0.f };
/// Inline static variables.



/**
 * @brief The Detail implemention of `Rotator` struct.
 */
#include "Rotator.inl"