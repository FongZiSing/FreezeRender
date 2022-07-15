//
// Ray.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of ray.
//
#pragma once

#include <Core/Math/Matrix.hpp>
#include "../RenderTarget.hpp"
#include "../Shading/Polygon.hpp"



namespace Pluto
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
		Vector3f origin;
		Vector3f direction;

		HitResult Intersect(const ShadingTriangle& triangle) const
		{
			constexpr float EPSILON = 0.000001f;

			const Vector3f& v1 = triangle.vertices[0].viewspace.position;
			const Vector3f& v2 = triangle.vertices[1].viewspace.position;
			const Vector3f& v3 = triangle.vertices[2].viewspace.position;

			const Vector3f e1 = v2 - v1;
			const Vector3f e2 = v3 - v1;
			const Vector3f s = origin - v1;
			const Vector3f s1 = direction ^ e2;
			const Vector3f s2 = s ^ e1;
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
		bool bDirty = false;

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
			}
		}

		constexpr const Ray& operator [] (const int& index) const
		{
			return data[index];
		}

		force_inline void Invalidate(const int& inWidth, const int& inHeight)
		{
			bDirty = true;
			Reallocate(inWidth, inHeight);
		}

		inline void TryRegenerateRay(const float& halfFOV, const float& aspectRatio)
		{
			if (bDirty)
			{
				int index = 0;
				float scale = std::tan(halfFOV);
				for (int h = 0; h < height; ++h)
				{
					for (int w = 0; w < width; ++w)
					{
						float x = (2.f * (w + 0.5f) / (float)width - 1.f) * aspectRatio * scale;
						float y = (1.f - 2 * (h + 0.5f) / (float)height) * scale;
						data[index++] = { Vector3f::Zero, Vector3f(x, y, -1).Normalize() };
					}
				}

				bDirty = false;
			}
		}
	};

}