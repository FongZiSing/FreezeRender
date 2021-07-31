#include <Utility/Math.hpp>



#ifndef ROTATOR_INL_ROTATOR_IPML
#define ROTATOR_INL_ROTATOR_IPML

	force_inline Rotator& Rotator::operator = (const Rotator& inValue) { yaw = inValue.yaw; pitch = inValue.pitch; roll = inValue.roll; return *this; }

	force_inline Vector3 Rotator::Euler() { return Vector3(roll, pitch, yaw); }
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

	force_inline Matrix Rotator::ToMatrix() const
	{
		// NOTE: intrinsic rotation.
		Matrix result;
		Vector4 radians;
		Vector4 degrees { 0, yaw, pitch, roll };
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

	force_inline Matrix Rotator::ToInvMatrix() const
	{
		Matrix result;
		Vector4 radians;
		Vector4 degrees { 0, yaw, pitch, roll };
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

	force_inline Vector3 Rotator::GetForwardVector() const
	{
		Vector3 result;
		Vector4 radians;
		Vector4 degrees { 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.x = -s[1] * c[2];
		result.y = s[2];
		result.z = -c[1] * c[2];

		return result;
	}

	force_inline Vector3 Rotator::GetRightVector() const
	{
		Vector3 result;
		Vector4 radians;
		Vector4 degrees { 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.x = c[1] * c[3] + s[1] * s[2] * s[3];
		result.y = c[2] * s[3];
		result.z = c[1] * s[2] * s[3] - s[1] * c[3];

		return result;
	}

	force_inline Vector3 Rotator::GetUpVector() const
	{
		Vector3 result;
		Vector4 radians;
		Vector4 degrees { 0, yaw, pitch, roll };
		Math::Degrees2Radians(&degrees, &radians);

		alignas(16) float s[4]; // ( Unused, yaw, pitch, roll )
		alignas(16) float c[4]; // ( Unused, yaw, pitch, roll )
		Math::VectorSinCos(&radians, s, c);

		result.x = s[1] * s[2] * c[3] - c[1] * s[3];
		result.y = c[2] * c[3];
		result.z = c[1] * s[2] * c[3] + s[1] * s[3];

		return result;
	}

#endif // !ROTATOR_INL_ROTATOR_IPML