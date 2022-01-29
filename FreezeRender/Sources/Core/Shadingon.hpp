#pragma once

#include <Common.hpp>
#include <Utility/Math.hpp>
#include "Meshlet.hpp"


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
	Vector3 position; // viewspace
	Vector3 normal;   // viewspace
	Vector2 uv;       // localspace
};



/**
 * @brief Vertex data for interpolating a shading point.
 */
struct alignas(16) ShadingVertex
{
	//! the memory order must be [ position(viewspace) - normal(viewspace) - uv(localspace) ]
	struct
	{
		Vector3 position;
		Vector3 normal;
	}
	viewspace;

	struct
	{
		Vector2 uv;
		Color color; // unused.
	}
	localspace;

	struct
	{
		Vector4 position;
	}
	screenspace;
};



/**
 * @brief 2d axis-aligned bounding box.
 */
struct alignas(16) ShadingBoundingBox
{
	int minX;
	int maxX;
	int minY;
	int maxY;
};



/**
 * @brief Triangle data for interpolating a shading point.
 */
struct ShadingTriangle
{
	// Counter-clockwise(CCW) vertices.
	ShadingVertex vertices[3];

	// Target sampling material.
	const Material* material = nullptr;


	ShadingTriangle() = default;

	ShadingTriangle(const ShadingTriangle&) = default;

	force_inline ShadingTriangle(const ShadingVertex& vertex1, const ShadingVertex& vertex2, const ShadingVertex& vertex3);

	force_inline ShadingTriangle(const Vertex* scope_restrict vertex1, const Vertex* scope_restrict vertex2, const Vertex* scope_restrict vertex3);

	warn_nodiscard force_inline ShadingBoundingBox BoundingBox(const int& width, const int& height) const;
	
	force_inline decltype(auto) ScreenspacePosition() const;

	force_inline decltype(auto) ViewspacePosition() const;

	force_inline decltype(auto) ViewspaceNormal() const;

	force_inline decltype(auto) LocalspaceUV() const;
};



/**
 * @brief Class for iterating vertex index buffer and creating `ShadingTriangle`.
 */
class ShadingMeshletIterator
{
	const VertexIndex* begin;
	const VertexIndex* end;

public:
	explicit ShadingMeshletIterator(const Meshlet* meshlet)
		: begin(meshlet->indices.data())
		, end(meshlet->indices.data() + meshlet->indices.size())
	{}

	explicit ShadingMeshletIterator(const Meshlet& meshlet)
		: begin(meshlet.indices.data())
		, end(meshlet.indices.data() + meshlet.indices.size())
	{}

	/// Iterative operations.
	force_inline void operator ++ () { begin += 3; }
	force_inline explicit operator bool() const { return begin < end; }
	force_inline bool operator ! () const { return !this->operator bool(); }
	/// Iterative operations.

	/**
	 * @brief triangle assembly.
	 */
	ShadingTriangle Assembly() const { return { begin[0].ptr, begin[1].ptr, begin[2].ptr }; }
};



#ifndef SHADINGON_HPP_OPERATION_IMPL
#define SHADINGON_HPP_OPERATION_IMPL

	/**
	 * @brief Returns ( vertex1 + alpha * (vertex2 - vertex1) ).
	 */
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

	force_inline ShadingTriangle::ShadingTriangle(const ShadingVertex& vertex1, const ShadingVertex& vertex2, const ShadingVertex& vertex3)
	{
		vertices[0] = vertex1;
		vertices[1] = vertex2;
		vertices[2] = vertex3;
	}

	force_inline ShadingTriangle::ShadingTriangle(const Vertex* scope_restrict vertex1, const Vertex* scope_restrict vertex2, const Vertex* scope_restrict vertex3)
	{
		// Initialize local space elements.
		// TODO: vertex color.
		//vertices[0].localspace.uv = vertex1->uv;
		//vertices[1].localspace.uv = vertex2->uv;
		//vertices[2].localspace.uv = vertex3->uv;

		//// Initialize view space elements.
		//vertices[0].viewspace.position = vertex1->position;
		//vertices[0].viewspace.normal = vertex1->normal;
		//vertices[1].viewspace.position = vertex2->position;
		//vertices[1].viewspace.normal = vertex2->normal;
		//vertices[2].viewspace.position = vertex3->position;
		//vertices[2].viewspace.normal = vertex3->normal;

		Register8Copy(vertex1, vertices + 0);
		Register8Copy(vertex2, vertices + 1);
		Register8Copy(vertex3, vertices + 2);

		// Initialize screenspace space elements.
		vertices[0].screenspace.position = { vertex1->position, 1.f };
		vertices[1].screenspace.position = { vertex2->position, 1.f };
		vertices[2].screenspace.position = { vertex3->position, 1.f };
	}

	force_inline ShadingBoundingBox ShadingTriangle::BoundingBox(const int& width, const int& height) const
	{
		const Vector4& v1 = vertices[0].screenspace.position;
		const Vector4& v2 = vertices[1].screenspace.position;
		const Vector4& v3 = vertices[2].screenspace.position;

		ShadingBoundingBox box;
		box.minX = (int)std::floor(std::min({ v1.x, v2.x, v3.x }));
		box.maxX = (int)std::ceil(std::max({ v1.x, v2.x, v3.x }));
		box.minY = (int)std::floor(std::min({ v1.y, v2.y, v3.y }));
		box.maxY = (int)std::ceil(std::max({ v1.y, v2.y, v3.y }));

		box.minX = std::max(         0, box.minX);
		box.maxX = std::min( width - 1, box.maxX);
		box.minY = std::max(         0, box.minY);
		box.maxY = std::min(height - 1, box.maxY);
		return box;
	}

	force_inline decltype(auto) ShadingTriangle::ScreenspacePosition() const {
		struct Batch { const Vector4& v1; const Vector4& v2; const Vector4& v3; };
		return Batch { vertices[0].screenspace.position, vertices[1].screenspace.position, vertices[2].screenspace.position };
	}

	force_inline decltype(auto) ShadingTriangle::ViewspacePosition() const
	{
		struct Batch { const Vector3& l1; const Vector3& l2; const Vector3& l3; };
		return Batch { vertices[0].viewspace.position, vertices[1].viewspace.position, vertices[2].viewspace.position };
	}

	force_inline decltype(auto) ShadingTriangle::ViewspaceNormal() const
	{
		struct Batch { const Vector3& n1; const Vector3& n2; const Vector3& n3; };
		return Batch { vertices[0].viewspace.normal, vertices[1].viewspace.normal, vertices[2].viewspace.normal };
	}

	force_inline decltype(auto) ShadingTriangle::LocalspaceUV() const
	{
		struct Batch { const Vector2& uv1; const Vector2& uv2; const Vector2& uv3; };
		return Batch { vertices[0].localspace.uv, vertices[1].localspace.uv, vertices[2].localspace.uv };
	}

#endif // !SHADINGON_HPP_OPERATION_IMPL
