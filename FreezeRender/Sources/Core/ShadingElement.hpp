#pragma once

#include <Common.hpp>
#include <Utility/Math.hpp>
#include "Matrix.hpp"
#include "Color.hpp"
#include "Meshlet.hpp"



/**
 * @brief Vertex data for interpolating a shading point.
 */
struct ShadingVertex
{
	struct
	{
		Vector2 uv;
		Color color; // unused.
	}
	localspace;

	struct
	{
		Vector3 position;
		Vector3 normal;
	}
	viewspace;

	struct
	{
		Vector4 position;
	}
	screenspace;


	/**
	 * @brief Returns ( vertex1 + alpha * (vertex2 - vertex1) ).
	 */
	force_inline static ShadingVertex Lerp(const float&alpha, const ShadingVertex& vertex1, const ShadingVertex& vertex2)
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
};



/**
 * @brief Triangle data for interpolating a shading point.
 */
struct ShadingTriangle
{
	// Counter-clockwise(CCW) vertices.
	ShadingVertex vertices[3];


	ShadingTriangle() = default;

	ShadingTriangle(const ShadingTriangle&) = default;

	force_inline ShadingTriangle(const ShadingVertex& vertex1, const ShadingVertex& vertex2, const ShadingVertex& vertex3)
	{
		vertices[0] = vertex1;
		vertices[1] = vertex2;
		vertices[2] = vertex3;
	}

	force_inline ShadingTriangle(const Vertex* vertex1, const Vertex* vertex2, const Vertex* vertex3)
	{
		// Initialize local space elements.
		// TODO: vertex color.
		vertices[0].localspace.uv = vertex1->uv;
		vertices[1].localspace.uv = vertex2->uv;
		vertices[2].localspace.uv = vertex3->uv;

		// Initialize view space elements.
		vertices[0].viewspace.position = vertex1->position;
		vertices[0].viewspace.normal = vertex1->normal;
		vertices[1].viewspace.position = vertex2->position;
		vertices[1].viewspace.normal = vertex2->normal;
		vertices[2].viewspace.position = vertex3->position;
		vertices[2].viewspace.normal = vertex3->normal;

		// Initialize screenspace space elements.
		vertices[0].screenspace.position = { vertex1->position, 1.f };
		vertices[1].screenspace.position = { vertex2->position, 1.f };
		vertices[2].screenspace.position = { vertex3->position, 1.f };
	}
};



/**
 * @brief Class for iterating vertex index buffer and creating `ShadingTriangle`.
 */
class ShadingIndexIterator
{
	const VertexIndex* indexPtr;
	const VertexIndex* indexEnd;

public:
	explicit ShadingIndexIterator(const Meshlet* meshlet)
		: indexPtr(meshlet->indices.data())
		, indexEnd(meshlet->indices.data() + meshlet->indices.size())
	{}

	explicit ShadingIndexIterator(const Meshlet& meshlet)
		: indexPtr(meshlet.indices.data())
		, indexEnd(meshlet.indices.data() + meshlet.indices.size())
	{}

	/// Iterative operations.
	force_inline void operator ++ () { indexPtr += 3; }
	force_inline explicit operator bool() const { return indexPtr < indexEnd; }
	force_inline bool operator ! () const { return !this->operator bool(); }
	/// Iterative operations.

	/**
	 * @brief triangle assembly.
	 */
	ShadingTriangle GetTriangle() const
	{
		return { indexPtr[0].ptr, indexPtr[1].ptr, indexPtr[2].ptr };
	}
};
