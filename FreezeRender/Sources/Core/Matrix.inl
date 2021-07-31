#include <Utility/Math.hpp>



#ifndef MATRIX_INL_VECTOR2_IPML
#define MATRIX_INL_VECTOR2_IPML

	force_inline Vector2& Vector2::operator = (const Vector2& inValue) noexcept {  x = inValue.x; y = inValue.y; return *this; }

	force_inline bool Vector2::operator == (const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
	force_inline bool Vector2::operator != (const Vector2& rhs) const { return x != rhs.x || y != rhs.y; }

	force_inline Vector2 Vector2::operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	force_inline Vector2 Vector2::operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	force_inline Vector2 Vector2::operator * (const Vector2& rhs) const { return Vector2(x * rhs.x, y * rhs.y); }
	force_inline Vector2 Vector2::operator / (const Vector2& rhs) const { return Vector2(x / rhs.x, y / rhs.y); }
	force_inline const Vector2& Vector2::operator += (const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
	force_inline const Vector2& Vector2::operator -= (const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	force_inline const Vector2& Vector2::operator *= (const Vector2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }
	force_inline const Vector2& Vector2::operator /= (const Vector2& rhs) { x /= rhs.x; y /= rhs.y; return *this; }

	force_inline Vector2 Vector2::operator + (const float& rhs) const { return Vector2(x + rhs, y + rhs); }
	force_inline Vector2 Vector2::operator - (const float& rhs) const { return Vector2(x - rhs, y - rhs); }
	force_inline Vector2 Vector2::operator * (const float& rhs) const { return Vector2(x * rhs, y * rhs); }
	force_inline Vector2 Vector2::operator / (const float& rhs) const { return Vector2(x / rhs, y / rhs); }
	force_inline const Vector2& Vector2::operator += (const float& rhs) { x += rhs; y += rhs; return *this; }
	force_inline const Vector2& Vector2::operator -= (const float& rhs) { x -= rhs; y -= rhs; return *this; }
	force_inline const Vector2& Vector2::operator *= (const float& rhs) { x *= rhs; y *= rhs; return *this; }
	force_inline const Vector2& Vector2::operator /= (const float& rhs) { x /= rhs; y /= rhs; return *this; }

	force_inline float Vector2::operator ^ (const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }
	force_inline float Vector2::operator | (const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }
	force_inline float Vector2::CrossProduct(const Vector2& rhs) const { return (*this) ^ rhs; }
	force_inline float Vector2::DotProduct(const Vector2& rhs) const { return (*this) | rhs; }
	force_inline float Vector2::Length() const { return std::sqrtf(x * x + y * y); }
	force_inline float Vector2::LengthSquared() const { return x * x + y * y; }

	force_inline Vector2 Vector2::Normalize(const float& tolerance) const
	{
		Vector2 result = *this;
		const float squareSum = result.x * result.x + result.y * result.y;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			result.x *= scale;
			result.y *= scale;
		}
		return result;
	}

	force_inline void Vector2::Normalized(const float& tolerance)
	{
		const float squareSum = x * x + y * y;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			x *= scale;
			y *= scale;
		}
	}

	force_inline bool Vector2::IsNormalized(const float& tolerance) const
	{
		const float squareSum = x * x + y * y;
		return (std::abs(1.f - squareSum) < tolerance);
	}

	force_inline Matrix Vector2::ToMatrix() const
	{
		return Matrix {
			{ 1, 0, 0, x },
			{ 0, 1, 0, y },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 },
		};
	}

	force_inline Matrix Vector2::ToInvMatrix() const
	{
		return Matrix {
			{ 1, 0, 0, -x },
			{ 0, 1, 0, -y },
			{ 0, 0, 1, 0  },
			{ 0, 0, 0, 1  },
		};
	}

#endif // !MATRIX_INL_VECTOR2_IPML



#ifndef MATRIX_INL_VECTOR3_IPML
#define MATRIX_INL_VECTOR3_IPML

	force_inline Vector3& Vector3::operator = (const Vector3& inValue) noexcept { x = inValue.x; y = inValue.y; z = inValue.z; return *this; }

	force_inline Vector2 Vector3::XY() { return Vector2(x, y); }
	force_inline Vector2 Vector3::XZ() { return Vector2(x, z); }
	force_inline Vector2 Vector3::YZ() { return Vector2(y, z); }
	force_inline Vector2 Vector3::XYRef() { return *(Vector2*)(&x); }
	force_inline Vector2 Vector3::YZRef() { return *(Vector2*)(&y); }

	force_inline bool Vector3::operator == (const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	force_inline bool Vector3::operator != (const Vector3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

	force_inline Vector3 Vector3::operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	force_inline Vector3 Vector3::operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	force_inline Vector3 Vector3::operator * (const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
	force_inline Vector3 Vector3::operator / (const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }
	force_inline const Vector3& Vector3::operator += (const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	force_inline const Vector3& Vector3::operator -= (const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	force_inline const Vector3& Vector3::operator *= (const Vector3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
	force_inline const Vector3& Vector3::operator /= (const Vector3& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

	force_inline Vector3 Vector3::operator + (const float& rhs) const { return Vector3(x + rhs, y + rhs, z + rhs); }
	force_inline Vector3 Vector3::operator - (const float& rhs) const { return Vector3(x - rhs, y - rhs, z - rhs); }
	force_inline Vector3 Vector3::operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	force_inline Vector3 Vector3::operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	force_inline const Vector3& Vector3::operator += (const float& rhs) { x += rhs; y += rhs; z += rhs; return *this; }
	force_inline const Vector3& Vector3::operator -= (const float& rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
	force_inline const Vector3& Vector3::operator *= (const float& rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
	force_inline const Vector3& Vector3::operator /= (const float& rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }

	force_inline Vector3 Vector3::operator ^ (const Vector3& rhs) const { return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }
	force_inline float Vector3::operator | (const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
	force_inline void Vector3::CrossProducted(const Vector3& rhs) { (*this) = (*this) ^ rhs; }
	force_inline Vector3 Vector3::CrossProduct(const Vector3& rhs) const { return (*this) ^ rhs; }
	force_inline float Vector3::DotProduct(const Vector3& rhs) const { return (*this) | rhs; }
	force_inline float Vector3::Length() const { return std::sqrtf(x * x + y * y + z * z); }
	force_inline float Vector3::LengthSquared() const { return x * x + y * y + z * z; }

	force_inline Vector3 Vector3::Normalize(const float& tolerance) const
	{
		Vector3 result = *this;
		const float squareSum = result.x * result.x + result.y * result.y + result.z * result.z;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			result.x *= scale;
			result.y *= scale;
			result.z *= scale;
		}
		return result;
	}

	force_inline void Vector3::Normalized(const float& tolerance)
	{
		const float squareSum = x * x + y * y + z * z;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			x *= scale;
			y *= scale;
			z *= scale;
		}
	}

	force_inline bool Vector3::IsNormalized(const float& tolerance) const
	{
		const float squareSum = x * x + y * y + z * z;
		return (std::abs(1.f - squareSum) < tolerance);
	}

	force_inline Matrix Vector3::ToMatrix() const
	{
		return Matrix{
			{ 1, 0, 0, x },
			{ 0, 1, 0, y },
			{ 0, 0, 1, z },
			{ 0, 0, 0, 1 },
		};
	}

	force_inline Matrix Vector3::ToInvMatrix() const
	{
		return Matrix{
			{ 1, 0, 0, -x },
			{ 0, 1, 0, -y },
			{ 0, 0, 1, -z },
			{ 0, 0, 0, 1  },
		};
	}

#endif // !MATRIX_INL_VECTOR3_IPML



#ifndef MATRIX_INL_VECTOR4_IPML
#define MATRIX_INL_VECTOR4_IPML

	static_assert(sizeof(Vector4) == sizeof(__m128), "[FreezeRender] Vector4 size is invalid!");

	force_inline Vector4& Vector4::operator = (const Vector4& inValue) noexcept { x = inValue.x; y = inValue.y; z = inValue.z; w = inValue.w; return *this; }

	force_inline Vector2 Vector4::XY() const { return Vector2(x, y); }
	force_inline Vector2 Vector4::ZW() const { return Vector2(z, w); }
	force_inline Vector3 Vector4::XYZ() const { return Vector3(x, y, z); }
	force_inline Vector3 Vector4::YZW() const { return Vector3(y, z, w); }
	force_inline Vector2& Vector4::XYRef() const { return *(Vector2*)(&x); }
	force_inline Vector2& Vector4::ZWRef() const { return *(Vector2*)(&z); }
	force_inline Vector3& Vector4::XYZRef() const { return *(Vector3*)(&x); }
	force_inline Vector3& Vector4::YZWRef() const { return *(Vector3*)(&y); }

	force_inline bool Vector4::operator == (const Vector4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
	force_inline bool Vector4::operator != (const Vector4& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }

	force_inline Vector4 Vector4::operator + (const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	force_inline Vector4 Vector4::operator - (const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	force_inline Vector4 Vector4::operator * (const Vector4& rhs) const { return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	force_inline Vector4 Vector4::operator / (const Vector4& rhs) const { return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
	force_inline const Vector4& Vector4::operator += (const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	force_inline const Vector4& Vector4::operator -= (const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	force_inline const Vector4& Vector4::operator *= (const Vector4& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
	force_inline const Vector4& Vector4::operator /= (const Vector4& rhs) { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }

	force_inline Vector4 Vector4::operator + (const float& rhs) const { return Vector4(x + rhs, y + rhs, z + rhs, w + rhs); }
	force_inline Vector4 Vector4::operator - (const float& rhs) const { return Vector4(x - rhs, y - rhs, z - rhs, w - rhs); }
	force_inline Vector4 Vector4::operator * (const float& rhs) const { return Vector4(x * rhs, y * rhs, z * rhs, w * rhs); }
	force_inline Vector4 Vector4::operator / (const float& rhs) const { return Vector4(x / rhs, y / rhs, z / rhs, w / rhs); }
	force_inline const Vector4& Vector4::operator += (const float& rhs) { x += rhs; y += rhs; z += rhs; w += rhs; return *this; }
	force_inline const Vector4& Vector4::operator -= (const float& rhs) { x -= rhs; y -= rhs; z -= rhs; w -= rhs; return *this; }
	force_inline const Vector4& Vector4::operator *= (const float& rhs) { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
	force_inline const Vector4& Vector4::operator /= (const float& rhs) { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }

	force_inline float Vector4::operator | (const Vector4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }
	force_inline float Vector4::DotProduct(const Vector4& rhs) const { return (*this) | rhs; }
	force_inline float Vector4::Length() const { return std::sqrtf(x * x + y * y + z * z + w * w); }
	force_inline float Vector4::LengthSquared() const { return x * x + y * y + z * z + w * w; }

	force_inline Vector4 Vector4::Normalize(const float& tolerance) const
	{
		Vector4 result = *this;
		const float squareSum = result.x * result.x + result.y * result.y + result.z * result.z + result.w * result.w;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			result.x *= scale;
			result.y *= scale;
			result.z *= scale;
			result.w *= scale;
		}
		return result;
	}

	force_inline void Vector4::Normalized(const float& tolerance)
	{
		const float squareSum = x * x + y * y + z * z + w * w;
		if (squareSum > tolerance)
		{
			const float scale = Math::InvSqrt(squareSum);
			x *= scale;
			y *= scale;
			z *= scale;
			w *= scale;
		}
	}

	force_inline bool Vector4::IsNormalized(const float& tolerance) const
	{
		const float squareSum = x * x + y * y + z * z + w * w;
		return (std::abs(1.f - squareSum) < Number::SMALL_NUMBER);
	}

#endif // !MATRIX_INL_VECTOR4_IPML



#ifndef MATRIX_INL_MATRIX_IPML
#define MATRIX_INL_MATRIX_IPML

	static_assert(sizeof(Matrix) == sizeof(__m128) * 4, "[FreezeRender] matrix size is invalid!");

	force_inline Matrix::Matrix(const float& value) noexcept
	{
		m[0][0] = value;  m[0][1] = 0.f;    m[0][2] = 0.f;    m[0][3] = 0.f;
		m[1][0] = 0.f;    m[1][1] = value;  m[1][2] = 0.f;    m[1][3] = 0.f;
		m[2][0] = 0.f;    m[2][1] = 0.f;    m[2][2] = value;  m[2][3] = 0.f;
		m[3][0] = 0.f;    m[3][1] = 0.f;    m[3][2] = 0.f;    m[3][3] = 1.f;
	}

	force_inline Matrix::Matrix(const Vector3& row0, const Vector3& row1, const Vector3& row2, const Vector3& row3)
	{
		m[0][0] = row0.x; m[0][1] = row0.y;  m[0][2] = row0.z;  m[0][3] = 0.f;
		m[1][0] = row1.x; m[1][1] = row1.y;  m[1][2] = row1.z;  m[1][3] = 0.f;
		m[2][0] = row2.x; m[2][1] = row2.y;  m[2][2] = row2.z;  m[2][3] = 0.f;
		m[3][0] = row3.x; m[3][1] = row3.y;  m[3][2] = row3.z;  m[3][3] = 1.f;
	}

	force_inline Matrix::Matrix(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
	{
		m[0][0] = row0.x; m[0][1] = row0.y;  m[0][2] = row0.z;  m[0][3] = row0.w;
		m[1][0] = row1.x; m[1][1] = row1.y;  m[1][2] = row1.z;  m[1][3] = row1.w;
		m[2][0] = row2.x; m[2][1] = row2.y;  m[2][2] = row2.z;  m[2][3] = row2.w;
		m[3][0] = row3.x; m[3][1] = row3.y;  m[3][2] = row3.z;  m[3][3] = row3.w;
	}

	force_inline Matrix::Matrix(const Matrix& value)
	{
		VectorCopy(&value.m[0][0], &m[0][0]);
		VectorCopy(&value.m[1][0], &m[1][0]);
		VectorCopy(&value.m[2][0], &m[2][0]);
		VectorCopy(&value.m[3][0], &m[3][0]);
	}

	force_inline Matrix& Matrix::operator = (const Matrix& value)
	{
		VectorCopy(&value.m[0][0], &m[0][0]);
		VectorCopy(&value.m[1][0], &m[1][0]);
		VectorCopy(&value.m[2][0], &m[2][0]);
		VectorCopy(&value.m[3][0], &m[3][0]);
		return *this;
	}

	force_inline Matrix::Matrix(const Matrix&& value) noexcept
	{
		m[0][0] = value.m[0][0]; m[0][1] = value.m[0][1];  m[0][2] = value.m[0][2];  m[0][3] = value.m[0][3];
		m[1][0] = value.m[1][0]; m[1][1] = value.m[1][1];  m[1][2] = value.m[1][2];  m[1][3] = value.m[1][3];
		m[2][0] = value.m[2][0]; m[2][1] = value.m[2][1];  m[2][2] = value.m[2][2];  m[2][3] = value.m[2][3];
		m[3][0] = value.m[3][0]; m[3][1] = value.m[3][1];  m[3][2] = value.m[3][2];  m[3][3] = value.m[3][3];
	}

	force_inline Matrix& Matrix::operator = (const Matrix&& value) noexcept
	{
		m[0][0] = value.m[0][0]; m[0][1] = value.m[0][1];  m[0][2] = value.m[0][2];  m[0][3] = value.m[0][3];
		m[1][0] = value.m[1][0]; m[1][1] = value.m[1][1];  m[1][2] = value.m[1][2];  m[1][3] = value.m[1][3];
		m[2][0] = value.m[2][0]; m[2][1] = value.m[2][1];  m[2][2] = value.m[2][2];  m[2][3] = value.m[2][3];
		m[3][0] = value.m[3][0]; m[3][1] = value.m[3][1];  m[3][2] = value.m[3][2];  m[3][3] = value.m[3][3];
		return *this;
	}

	force_inline bool Matrix::operator == (const Matrix& rhs) const
	{
		const float* scope_restrict lhsPtr = &m[0][0];
		const float* scope_restrict rhsPtr = &rhs.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++)
			if (*rhsPtr != *lhsPtr) return false;
		return true;
	}

	force_inline bool Matrix::operator != (const Matrix& rhs) const
	{
		return !(*this == rhs);
	}

	force_inline Matrix Matrix::operator + (const Matrix& rhs) const
	{
		Matrix result;
		const float* scope_restrict lhsPtr = &m[0][0];
		const float* scope_restrict rhsPtr = &rhs.m[0][0];
		float* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++, outPtr++)
			*outPtr = *lhsPtr + *rhsPtr;
		return result;
	}

	force_inline Matrix Matrix::operator - (const Matrix& rhs) const
	{
		Matrix result;
		const float* scope_restrict lhsPtr = &m[0][0];
		const float* scope_restrict rhsPtr = &rhs.m[0][0];
		float* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++, outPtr++)
			*outPtr = *lhsPtr - *rhsPtr;
		return result;
	}

	force_inline Matrix Matrix::operator * (const Matrix& rhs) const
	{
		Matrix result;
		Math::MatrixMultiplyMatrix(this, &rhs, &result);
		return result;
	}

	force_inline const Matrix& Matrix::operator += (const Matrix& rhs)
	{
		float* scope_restrict lhsPtr = &m[0][0];
		const float* scope_restrict rhsPtr = &rhs.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++)
			*lhsPtr += *rhsPtr;
		return *this;
	}

	force_inline const Matrix& Matrix::operator -= (const Matrix& rhs)
	{
		float* scope_restrict lhsPtr = &m[0][0];
		const float* scope_restrict rhsPtr = &rhs.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, rhsPtr++)
			*lhsPtr -= *rhsPtr;
		return *this;
	}

	force_inline const Matrix& Matrix::operator *= (const Matrix& rhs)
	{
		Math::MatrixMultiplyMatrix(this, &rhs, this);
		return *this;
	}

	force_inline Matrix Matrix::Transpose() const
	{
		Matrix result = *this;
		Math::MatrixTranspose(&result);
		return result;
	}

	force_inline Matrix Matrix::Inverse() const
	{
		Matrix result;
		Math::MatrixInverse(this, &result);
		return result;
	}

	force_inline Matrix Matrix::operator * (const float& rhs) const
	{
		Matrix result;
		const float* scope_restrict lhsPtr = &m[0][0];
		float* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, outPtr++)
			*outPtr = *lhsPtr * rhs;
		return result;
	}

	force_inline Matrix Matrix::operator / (const float& rhs) const
	{
		Matrix result;
		const float* scope_restrict lhsPtr = &m[0][0];
		float* scope_restrict outPtr = &result.m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++, outPtr++)
			*outPtr = *lhsPtr / rhs;
		return result;
	}

	force_inline const Matrix& Matrix::operator *= (const float& rhs)
	{
		float* scope_restrict lhsPtr = &m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++)
			*lhsPtr *= rhs;
		return *this;
	}

	force_inline const Matrix& Matrix::operator /= (const float& rhs)
	{
		float* scope_restrict lhsPtr = &m[0][0];

		for (int i = 0; i < 16; i++, lhsPtr++)
			*lhsPtr /= rhs;
		return *this;
	}

	force_inline Vector3 Matrix::operator * (const Vector3& rhs) const
	{
		Vector4 result(rhs, 1.f);
		Math::MatrixMulitplyVectorH(this, &result, &result);
		return result.XYZ();
	}

	force_inline Vector4 Matrix::operator * (const Vector4& rhs) const
	{
		Vector4 result;
		Math::MatrixMulitplyVector(this, &rhs, &result);
		return result;
	}

#endif // !MATRIX_INL_MATRIX_IPML