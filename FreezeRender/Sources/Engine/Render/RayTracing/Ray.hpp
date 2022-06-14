#pragma once

#include <Core/Math/Matrix.hpp>
#include "../RenderTarget.hpp"
#include "../Shading/Polygon.hpp"



enum class HitStatus : unsigned int
{
	Miss = 0,
	Success,
	Parallel,
};



struct alignas(16) HitResult
{
	HitStatus status;
	float t;          // Ray = origin + t * direction
	float beta;       // equals v, for barycentric Interpolation.
	float gamma;      // equals w, for barycentric Interpolation.

	HitResult()
		: status(HitStatus::Miss)
		, t(Number::FLOAT_INF)
		, beta(Number::FLOAT_INF)
		, gamma(Number::FLOAT_INF)
	{}

	bool operator == (const HitStatus& rhs)
	{
		return status == rhs;
	}
};



struct Ray
{
	Vector3 origin;
	Vector3 direction;

	HitResult Intersect(const ShadingTriangle& triangle) const
	{
		constexpr float EPSILON = 0.000001f;

		const Vector3& v1 = triangle.vertices[0].viewspace.position;
		const Vector3& v2 = triangle.vertices[1].viewspace.position;
		const Vector3& v3 = triangle.vertices[2].viewspace.position;

		const Vector3 e1 = v2 - v1;
		const Vector3 e2 = v3 - v1;
		const Vector3 s = origin - v1;
		const Vector3 s1 = direction ^ e2;
		const Vector3 s2 = s ^ e1;
		const float coeff = s1 | e1;


		HitResult result{};
		float& t = result.t;
		float& v = result.beta;
		float& w = result.gamma;

		if (std::abs(coeff) < EPSILON)
		{
			// This ray is parallel to this triangle.
			result.status = HitStatus::Parallel;
			return result;
		}

		const float inv = 1.f / coeff;

		// Calculate and verify alpha.
		v = inv * (s1 | s);
		if (v < 0 || v > 1.f)
		{
			return result;
		}

		// Calculate and verify beta.
		w = inv * (s2 | direction);
		if (w < 0 || v + w > 1.f)
		{
			return result;
		}

		// Calculate and verify t.
		t = inv * (s2 | e2);
		if (t > EPSILON)
		{
			result.status = HitStatus::Success;
			return result;
		}

		// This means that there is a line intersection but not a ray intersection.
		return result;
	}
};



class PrimitiveRay
{
protected:
	Bulkdata<Ray> data;
	int width = 0;
	int height = 0;

public:
	explicit PrimitiveRay(const int& inWidth, const int& inHeight)
	{
		Reallocate(inWidth, inHeight);
	}

	inline void Reallocate(const int& inWidth, const int& inHeight)
	{
		if (inWidth > 0 && inHeight > 0)
		{
			width = inWidth;
			height = inHeight;
			data.Reallocate(1LL * inWidth * inHeight);
			data.Initialize(Ray{});
		}
	}

	inline void Reset()
	{
		data.Initialize(Ray{});
	}

	force_inline void Swap(PrimitiveRay& rhs)
	{
		std::swap(width, rhs.width);
		std::swap(height, rhs.height);
		data.Swap(rhs.data);
	}
	constexpr Ray& operator [] (const int& index)
	{
		return data[index];
	}
		
	constexpr const Ray& operator [] (const int& index) const
	{
		return data[index];
	}
};



