#pragma once

#include <Core/Math/Matrix.hpp>
#include "../RenderTarget.hpp"
#include "../Shading/Polygon.hpp"


namespace Shading
{
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
		float alpha;      // equals u, for barycentric Interpolation.
		float beta;       // equals v, for barycentric Interpolation.

		HitResult()
			: status(HitStatus::Miss)
			, t(Number::FLOAT_INF)
			, alpha(Number::FLOAT_INF)
			, beta(Number::FLOAT_INF)
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
			float& u = result.alpha;
			float& v = result.beta;

			if (std::abs(coeff) < EPSILON)
			{
				// This ray is parallel to this triangle.
				result.status = HitStatus::Parallel;
				return result;
			}

			const float inv = 1.f / coeff;

			// Calculate and verify alpha.
			u = inv * (s1 | s);
			if (u < 0 || u > 1.f)
			{
				return result;
			}

			// Calculate and verify beta.
			v = inv * (s2 | direction);
			if (v < 0 || u + v > 1.f)
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
}


struct RayPixelTraits
{
	using Type = Shading::Ray;
	inline static const Type defaultPixelValue = {};
}; typedef RenderTarget<RayPixelTraits> PrimitiveRay;



