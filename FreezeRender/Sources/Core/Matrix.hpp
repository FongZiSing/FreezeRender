#pragma once

#include <Common.hpp>
#include <Utility/Number.hpp>

/// forward declaration.
struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix;
/// forward declaration.



struct Vector2
{
	float x, y;


	/// Constants.
	static const Vector2 Zero;
	static const Vector2 One;
	/// Constants.


	/// Initialize.
	Vector2() noexcept : x(0.f), y(0.f) {}
	Vector2(const float& inValue) noexcept : x(inValue), y(inValue) {}
	Vector2(const float& inX, const float& inY) noexcept : x(inX), y(inY) {}
	Vector2(const Vector2& inValue) : x(inValue.x), y(inValue.y) {}
	force_inline Vector2& operator = (const Vector2& inValue) noexcept;
	/// Initialize.


	/// Self-related operations.
	force_inline bool operator == (const Vector2& rhs) const;
	force_inline bool operator != (const Vector2& rhs) const;

	force_inline Vector2 operator + (const Vector2& rhs) const;
	force_inline Vector2 operator - (const Vector2& rhs) const;
	force_inline Vector2 operator * (const Vector2& rhs) const;
	force_inline Vector2 operator / (const Vector2& rhs) const;
	force_inline const Vector2& operator += (const Vector2& rhs);
	force_inline const Vector2& operator -= (const Vector2& rhs);
	force_inline const Vector2& operator *= (const Vector2& rhs);
	force_inline const Vector2& operator /= (const Vector2& rhs);
	/// Self-related operations.


	/// Number-related operations.
	force_inline Vector2 operator + (const float& rhs) const;
	force_inline Vector2 operator - (const float& rhs) const;
	force_inline Vector2 operator * (const float& rhs) const;
	force_inline Vector2 operator / (const float& rhs) const;
	force_inline const Vector2& operator += (const float& rhs);
	force_inline const Vector2& operator -= (const float& rhs);
	force_inline const Vector2& operator *= (const float& rhs);
	force_inline const Vector2& operator /= (const float& rhs);
	/// Number-related operations.


	/// Vector operations. 
	force_inline float operator ^ (const Vector2& rhs) const;
	force_inline float operator | (const Vector2& rhs) const;
	warn_nodiscard force_inline float CrossProduct(const Vector2& rhs) const;
	warn_nodiscard force_inline float DotProduct(const Vector2& rhs) const;
	warn_nodiscard force_inline float Length() const;
	warn_nodiscard force_inline float LengthSquared() const;
	/// Vector operations. 


	/**
	 * @brief Normalization.
	 * @param tolerance    - error measure.
	 */
	warn_nodiscard force_inline Vector2 Normalize(const float& tolerance = Number::SMALL_NUMBER) const;

	/**
	 * @brief Self-normalization. the same as `*this = Normalize(tolerance);`
	 * @param tolerance    - error measure.
	 */
	force_inline void Normalized(const float& tolerance = Number::SMALL_NUMBER);

	/**
	 * @brief Determines whether it is a unit vector.
	 */
	warn_nodiscard force_inline bool IsNormalized(const float& tolerance = Number::SMALL_NUMBER) const;

	/**
	 * @brief Convert vector to translation matrix.
	 */
	warn_nodiscard force_inline Matrix ToMatrix() const;

	/**
	 * @brief Convert vector to inverse translation matrix.
	 */
	warn_nodiscard force_inline Matrix ToInvMatrix() const;
};



struct Vector3
{
	float x, y, z;


	/// Constants.
	static const Vector3 Zero;
	static const Vector3 One;
	/// Constants.


	/// Initialize.
	Vector3() noexcept : x(0.f), y(0.f), z(0.f) {}
	Vector3(const float& inValue) noexcept : x(inValue), y(inValue), z(inValue) {}
	Vector3(const float& inX, const float& inY, const float& inZ) noexcept : x(inX), y(inY), z(inZ) {}
	Vector3(const Vector2& inValue, const float& inZ = 0.f) : x(inValue.x), y(inValue.y), z(inZ) {}
	Vector3(const float& inX, const Vector2& inValue) : x(inX), y(inValue.x), z(inValue.y) {}
	Vector3(const Vector3& inValue) : x(inValue.x), y(inValue.y), z(inValue.z) {}
	force_inline Vector3& operator = (const Vector3& inValue) noexcept;
	/// Initialize.


	/// Self-related operations.
	force_inline Vector2 XY();
	force_inline Vector2 XZ();
	force_inline Vector2 YZ();
	force_inline Vector2 XYRef();
	force_inline Vector2 YZRef();

	force_inline bool operator == (const Vector3& rhs) const;
	force_inline bool operator != (const Vector3& rhs) const;

	force_inline Vector3 operator + (const Vector3& rhs) const;
	force_inline Vector3 operator - (const Vector3& rhs) const;
	force_inline Vector3 operator * (const Vector3& rhs) const;
	force_inline Vector3 operator / (const Vector3& rhs) const;
	force_inline const Vector3& operator += (const Vector3& rhs);
	force_inline const Vector3& operator -= (const Vector3& rhs);
	force_inline const Vector3& operator *= (const Vector3& rhs);
	force_inline const Vector3& operator /= (const Vector3& rhs);
	/// Self-related operations.


	/// Number-related operations.
	force_inline Vector3 operator + (const float& rhs) const;
	force_inline Vector3 operator - (const float& rhs) const;
	force_inline Vector3 operator * (const float& rhs) const;
	force_inline Vector3 operator / (const float& rhs) const;
	force_inline const Vector3& operator += (const float& rhs);
	force_inline const Vector3& operator -= (const float& rhs);
	force_inline const Vector3& operator *= (const float& rhs);
	force_inline const Vector3& operator /= (const float& rhs);
	/// Number-related operations.


	/// Vector operations.
	force_inline Vector3 operator ^ (const Vector3& rhs) const;
	force_inline float operator | (const Vector3& rhs) const;
	force_inline void CrossProducted(const Vector3& rhs);
	force_inline void DotProducted(const Vector3& rhs);
	warn_nodiscard force_inline Vector3 CrossProduct(const Vector3& rhs) const;
	warn_nodiscard force_inline float DotProduct(const Vector3& rhs) const;
	warn_nodiscard force_inline float Length() const;
	warn_nodiscard force_inline float LengthSquared() const;
	/// Vector operations.


	/**
	 * @brief Normalization.
	 * @param tolerance    - error measure.
	 */
	warn_nodiscard force_inline Vector3 Normalize(const float& tolerance = Number::SMALL_NUMBER) const;

	/**
	 * @brief Self-normalization. the same as `*this = Normalize(tolerance);`
	 * @param tolerance    - error measure.
	 */
	force_inline void Normalized(const float& tolerance = Number::SMALL_NUMBER);

	/**
	 * @brief Determines whether it is a unit vector.
	 */
	warn_nodiscard force_inline bool IsNormalized(const float& tolerance = Number::SMALL_NUMBER) const;

	/**
	 * @brief Convert vector to translation matrix.
	 */
	warn_nodiscard force_inline Matrix ToMatrix() const;

	/**
	 * @brief Convert vector to inverse translation matrix.
	 */
	warn_nodiscard force_inline Matrix ToInvMatrix() const;
};



struct alignas(16) Vector4
{
	float x, y, z, w;


	/// Constants.
	static const Vector4 Zero;
	static const Vector4 One;
	/// Constants.


	/// Initialize.
	Vector4() noexcept : x(0.f), y(0.f), z(0.f), w(0.f) {}
	Vector4(const float& inValue) noexcept : x(inValue), y(inValue), z(inValue), w(inValue) {}
	Vector4(const float& inX, const float& inY, const float& inZ, const float& inW) noexcept : x(inX), y(inY), z(inZ), w(inW) {}
	Vector4(const Vector2& inValue, const float& inZ = 0.f, const float& inW = 0.f) : x(inValue.x), y(inValue.y), z(inZ), w(inW) {}
	Vector4(const float& inX, const float& inY, const Vector2& inValue) : x(inX), y(inY), z(inValue.x), w(inValue.y) {}
	Vector4(const Vector2& in0, const Vector2& in1) : x(in0.x), y(in0.y), z(in1.x), w(in1.y) {}
	Vector4(const float& inX, const Vector3& inValue) : x(inX), y(inValue.x), z(inValue.y), w(inValue.z) {}
	Vector4(const Vector3& inValue, const float& inW = 0.f) : x(inValue.x), y(inValue.y), z(inValue.z), w(inW) {}
	Vector4(const Vector4& inValue) : x(inValue.x), y(inValue.y), z(inValue.z), w(inValue.w) {}
	force_inline Vector4& operator = (const Vector4& inValue) noexcept;
	/// Initialize.


	/// Self-related operations.
	force_inline Vector2 XY() const;
	force_inline Vector2 ZW() const;
	force_inline Vector3 XYZ() const;
	force_inline Vector3 YZW() const;
	force_inline Vector2& XYRef() const;
	force_inline Vector2& ZWRef() const;
	force_inline Vector3& XYZRef() const;
	force_inline Vector3& YZWRef() const;

	force_inline bool operator == (const Vector4& rhs) const;
	force_inline bool operator != (const Vector4& rhs) const;

	force_inline Vector4 operator + (const Vector4& rhs) const;
	force_inline Vector4 operator - (const Vector4& rhs) const;
	force_inline Vector4 operator * (const Vector4& rhs) const;
	force_inline Vector4 operator / (const Vector4& rhs) const;
	force_inline const Vector4& operator += (const Vector4& rhs);
	force_inline const Vector4& operator -= (const Vector4& rhs);
	force_inline const Vector4& operator *= (const Vector4& rhs);
	force_inline const Vector4& operator /= (const Vector4& rhs);
	/// Self-related operations.


	/// Number-related operations.
	force_inline Vector4 operator + (const float& rhs) const;
	force_inline Vector4 operator - (const float& rhs) const;
	force_inline Vector4 operator * (const float& rhs) const;
	force_inline Vector4 operator / (const float& rhs) const;
	force_inline const Vector4& operator += (const float& rhs);
	force_inline const Vector4& operator -= (const float& rhs);
	force_inline const Vector4& operator *= (const float& rhs);
	force_inline const Vector4& operator /= (const float& rhs);
	/// Number-related operations.


	/// Vector operations.
	force_inline float operator | (const Vector4& rhs) const;
	force_inline void DotProducted(const Vector4& rhs);
	force_inline float DotProduct(const Vector4& rhs) const;
	warn_nodiscard force_inline float Length() const;
	warn_nodiscard force_inline float LengthSquared() const;
	/// Vector operations.


	/**
	 * @brief Normalization.
	 * @param tolerance    - error measure.
	 */
	warn_nodiscard force_inline Vector4 Normalize(const float& tolerance = Number::SMALL_NUMBER) const;

	/**
	 * @brief Self-normalization. the same as `*this = Normalize(tolerance);`
	 * @param tolerance    - error measure.
	 */
	force_inline void Normalized(const float& tolerance = Number::SMALL_NUMBER);

	/**
	 * @brief Determines whether it is a unit vector.
	 */
	warn_nodiscard force_inline bool IsNormalized(const float& tolerance = Number::SMALL_NUMBER) const;
};



struct Matrix
{
	union
	{
		alignas(16) float m[4][4];
	};


	/// Constants.
	static const Matrix Identity;
	/// Constants.


	/// Initialize.
	Matrix() = default;
	force_inline Matrix(const float& value) noexcept;
	force_inline Matrix(const Vector3& row0, const Vector3& row1, const Vector3& row2, const Vector3& row3);
	force_inline Matrix(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
	force_inline Matrix(const Matrix& value);
	force_inline Matrix& operator = (const Matrix& value);
	force_inline Matrix(const Matrix&& value) noexcept;
	force_inline Matrix& operator = (const Matrix&& value) noexcept;
	/// Initialize.


	/// Self-related operations.
	force_inline bool operator == (const Matrix& rhs) const;
	force_inline bool operator != (const Matrix& rhs) const;

	force_inline Matrix operator + (const Matrix& rhs) const;
	force_inline Matrix operator - (const Matrix& rhs) const;
	force_inline Matrix operator * (const Matrix& rhs) const;
	force_inline const Matrix& operator += (const Matrix& rhs);
	force_inline const Matrix& operator -= (const Matrix& rhs);
	force_inline const Matrix& operator *= (const Matrix& rhs);

	warn_nodiscard force_inline Matrix Transpose() const;
	warn_nodiscard force_inline Matrix Inverse() const;
	/// Self-related operations.


	/// Number-related operations.
	force_inline Matrix operator * (const float& rhs) const;
	force_inline Matrix operator / (const float& rhs) const;
	force_inline const Matrix& operator *= (const float& rhs);
	force_inline const Matrix& operator /= (const float& rhs);
	/// Number-related operations.


	/// Vector operations.
	warn_deprecated("** Performance issues **") force_inline Vector3 operator * (const Vector3& rhs) const;
	warn_deprecated("** Performance issues **") force_inline Vector4 operator * (const Vector4& rhs) const;
	/// Vector operations.
};



/// Inline static variables.
const inline_variable Vector2 Vector2::One = { 1.f, 1.f };
const inline_variable Vector2 Vector2::Zero = {};

const inline_variable Vector3 Vector3::Zero = {};
const inline_variable Vector3 Vector3::One = { 1.f, 1.f, 1.f };

const inline_variable Vector4 Vector4::Zero = {};
const inline_variable Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };

const inline_variable Matrix Matrix::Identity = 1.f;
/// Inline static variables.



/**
 * @brief The Detail implemention of `Vector2`, `Vector3`, `Vector4` and `Matrix` struct.
 */
#include "Matrix.inl"