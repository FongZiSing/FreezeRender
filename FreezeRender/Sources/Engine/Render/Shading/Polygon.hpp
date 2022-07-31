//
// Polygon.hpp
//
//       Copyright (c) FreezeRender. All rights reserved.
//       @Author FongZiSing
//
// Implemention of polygon.
//
#pragma once

#include <Common.hpp>
#include <Core/Math/Math.hpp>
#include <Core/Math/Matrix.hpp>
#include <Core/Math/Color.hpp>
#include <Core/Math/Box.hpp>
#include <Asset/Meshlet/Meshlet.hpp>

#include "Material.hpp"



namespace Pluto
{
	/**
	 * @brief The interpolation coefficient of shading point.
	 */
	struct alignas(16) ShadingInterpolation
	{
		float oneOverDepth;
		float gamma;
		float alpha;
		float beta;
	};



	/**
	 * @brief The attributes of a shading point.
	 */
	struct alignas(16) ShadingPoint
	{
		Vector3f position; // viewspace
		Vector3f normal;   // viewspace
		Vector2f uv;       // localspace
	};



	/**
	 * @brief Vertex data for interpolating a shading point.
	 */
	struct alignas(16) ShadingVertex
	{
		//! the memory order must be [ position(viewspace) - normal(viewspace) - uv(localspace) ]
		struct
		{
			Vector3f position;
			Vector3f normal;
		}
		viewspace;

		struct
		{
			Vector2f uv;
			Color color; // unused.
		}
		localspace;

		struct
		{
			Vector4f position;
		}
		screenspace;
	};

	force_inline ShadingVertex Lerp(const float& alpha, const ShadingVertex& vertex1, const ShadingVertex& vertex2)
	{
		ShadingVertex result;

		// Interpolate local space elements.
		// TODO: vertex color.
		result.localspace.uv = Math::Lerp(alpha, vertex1.localspace.uv, vertex2.localspace.uv);

		// Interpolate view space elements.
		result.viewspace.position = Math::Lerp(alpha, vertex1.viewspace.position, vertex2.viewspace.position);
		result.viewspace.normal = Math::Lerp(alpha, vertex1.viewspace.normal, vertex2.viewspace.normal);

		// Interpolate screen space elements.
		Math::VectorLerp(alpha, &vertex1.screenspace.position, &vertex2.screenspace.position, &result.screenspace.position);

		return result;
	}



	/**
	 * @brief Triangle data for interpolating a shading point.
	 */
	struct ShadingTriangle
	{
		// Counter-clockwise(CCW) vertices.
		ShadingVertex vertices[3];

		// Target sampling material.
		const Material* material;


		ShadingTriangle() = default;

		~ShadingTriangle() = default;

		inline ShadingTriangle(const ShadingVertex& vertex1, const ShadingVertex& vertex2, const ShadingVertex& vertex3)
		{
			vertices[0] = vertex1;
			vertices[1] = vertex2;
			vertices[2] = vertex3;
		}

		inline ShadingTriangle(const AVertex& vertex1, const AVertex& vertex2, const AVertex& vertex3, const Material* target)
		{
			Register8Copy(&vertex1, vertices + 0);
			Register8Copy(&vertex2, vertices + 1);
			Register8Copy(&vertex3, vertices + 2);

			vertices[0].screenspace.position = { vertex1.position, 1.f };
			vertices[1].screenspace.position = { vertex2.position, 1.f };
			vertices[2].screenspace.position = { vertex3.position, 1.f };

			material = target;
		}

		warn_nodiscard inline Box2i GetBoundingBox(const int& width, const int& height) const
		{
			const Vector4f& v1 = vertices[0].screenspace.position;
			const Vector4f& v2 = vertices[1].screenspace.position;
			const Vector4f& v3 = vertices[2].screenspace.position;

			Box2i box;
			box.min.x = (int)std::floor(std::min({ v1.x, v2.x, v3.x }));
			box.max.x = (int)std::ceil(std::max({ v1.x, v2.x, v3.x }));
			box.min.y = (int)std::floor(std::min({ v1.y, v2.y, v3.y }));
			box.max.y = (int)std::ceil(std::max({ v1.y, v2.y, v3.y }));

			box.min.x = std::max(0,          box.min.x);
			box.max.x = std::min(width - 1,  box.max.x);
			box.min.y = std::max(0,          box.min.y);
			box.max.y = std::min(height - 1, box.max.y);
			return box;
		}

		inline decltype(auto) ScreenspacePosition() const
		{
			struct Batch { const Vector4f& v1; const Vector4f& v2; const Vector4f& v3; };
			return Batch{ vertices[0].screenspace.position, vertices[1].screenspace.position, vertices[2].screenspace.position };
		}

		inline decltype(auto) ViewspacePosition() const
		{
			struct Batch { const Vector3f& l1; const Vector3f& l2; const Vector3f& l3; };
			return Batch{ vertices[0].viewspace.position, vertices[1].viewspace.position, vertices[2].viewspace.position };
		}

		inline decltype(auto) ViewspaceNormal() const
		{
			struct Batch { const Vector3f& n1; const Vector3f& n2; const Vector3f& n3; };
			return Batch{ vertices[0].viewspace.normal, vertices[1].viewspace.normal, vertices[2].viewspace.normal };
		}

		inline decltype(auto) LocalspaceUV() const
		{
			struct Batch { const Vector2f& uv1; const Vector2f& uv2; const Vector2f& uv3; };
			return Batch{ vertices[0].localspace.uv, vertices[1].localspace.uv, vertices[2].localspace.uv };
		}
	};
}